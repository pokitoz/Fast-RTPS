// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file HelloWorldSubscriber.cpp
 *
 */

#include "HelloWorldSubscriber.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/utils/eClock.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/DataReader.hpp>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

HelloWorldSubscriber::HelloWorldSubscriber()
    : participant_(nullptr)
    , subscriber_(nullptr)
    , type_(new HelloWorldPubSubType())
{
}

bool HelloWorldSubscriber::init()
{
    ParticipantAttributes participant_att;
    participant_att.rtps.builtin.domainId = 0;
    participant_att.rtps.setName("Participant_sub");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(participant_att);

    if (participant_ == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    type_.register_type(participant_, type_->getName());

    //CREATE THE SUBSCRIBER
    SubscriberAttributes sub_att;
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, sub_att, nullptr);

    if (subscriber_ == nullptr)
    {
        return false;
    }

    // CREATE THE READER
    ReaderQos rqos;
    rqos.m_durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
    rqos.m_reliability.kind = RELIABLE_RELIABILITY_QOS;
    TopicAttributes topic_att;
    topic_att.topicDataType = "HelloWorld";
    topic_att.topicName = "HelloWorldTopic";
    topic_att.historyQos.kind = KEEP_LAST_HISTORY_QOS;
    topic_att.historyQos.depth = 30;
    topic_att.resourceLimitsQos.max_samples = 50;
    topic_att.resourceLimitsQos.allocated_samples = 20;
    reader_ = subscriber_->create_datareader(topic_att, rqos, &listener_);

    if (reader_ == nullptr)
    {
        return false;
    }

    return true;
}

HelloWorldSubscriber::~HelloWorldSubscriber()
{
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void HelloWorldSubscriber::SubListener::on_subscription_matched(
        eprosima::fastdds::dds::DataReader*,
        eprosima::fastdds::dds::SubscriptionMatchedStatus& info)
{
    if (info.status == MATCHED_MATCHING)
    {
        matched++;
        std::cout << "Subscriber matched." << std::endl;
    }
    else
    {
        matched--;
        std::cout << "Subscriber unmatched." << std::endl;
    }
}

void HelloWorldSubscriber::SubListener::on_data_available(
        eprosima::fastdds::dds::DataReader* reader)
{
    if (reader->take_next_sample(&hello, &info))
    {
        if (info.sampleKind == ALIVE)
        {
            samples++;
            // Print your structure data here.
            std::cout << "Message " << hello.message() << " " << hello.index() << " RECEIVED" << std::endl;
        }
    }
}

void HelloWorldSubscriber::run()
{
    std::cout << "Subscriber running. Please press enter to stop the Subscriber" << std::endl;
    std::cin.ignore();
}

void HelloWorldSubscriber::run(
        uint32_t number)
{
    std::cout << "Subscriber running until " << number << "samples have been received" << std::endl;
    while (number > listener_.samples)
    {
        eClock::my_sleep(500);
    }
}