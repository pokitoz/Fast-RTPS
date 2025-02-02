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
 * @file WriterAttributes.h
 *
 */
#ifndef WRITERATTRIBUTES_H_
#define WRITERATTRIBUTES_H_

#include "../common/Time_t.h"
#include "../common/Guid.h"
#include "../flowcontrol/ThroughputControllerDescriptor.h"
#include "EndpointAttributes.h"
#include "../../utils/collections/ResourceLimitedContainerConfig.hpp"
#include "../../qos/QosPolicies.h"

#include <functional>

namespace eprosima{
namespace fastrtps{
namespace rtps{

class ReaderProxyData;

typedef enum RTPSWriterPublishMode : octet
{
    SYNCHRONOUS_WRITER,
    ASYNCHRONOUS_WRITER
} RTPSWriterPublishMode;


/**
 * Struct WriterTimes, defining the times associated with the Reliable Writers events.
 * @ingroup RTPS_ATTRIBUTES_MODULE
 */
struct WriterTimes
{
    //! Initial heartbeat delay. Default value ~11ms.
    Duration_t initialHeartbeatDelay;
    //! Periodic HB period, default value 3s.
    Duration_t heartbeatPeriod;
    //!Delay to apply to the response of a ACKNACK message, default value ~5ms.
    Duration_t nackResponseDelay;
    //!This time allows the RTPSWriter to ignore nack messages too soon after the data as sent, default value 0s.
    Duration_t nackSupressionDuration;

    WriterTimes()
    {
        //initialHeartbeatDelay.fraction = 50*1000*1000;
        initialHeartbeatDelay.nanosec = 12*1000*1000;
        heartbeatPeriod.seconds = 3;
        //nackResponseDelay.fraction = 20*1000*1000;
        nackResponseDelay.nanosec = 5*1000*1000;
    }

    virtual ~WriterTimes() {}

    bool operator==(const WriterTimes& b) const
    {
        return (this->initialHeartbeatDelay == b.initialHeartbeatDelay) &&
               (this->heartbeatPeriod == b.heartbeatPeriod) &&
               (this->nackResponseDelay == b.nackResponseDelay) &&
               (this->nackSupressionDuration == b.nackSupressionDuration);
    }
};

/**
 * Class WriterAttributes, defining the attributes of a RTPSWriter.
 * @ingroup RTPS_ATTRIBUTES_MODULE
 */
class WriterAttributes
{
    public:

        WriterAttributes()
            : liveliness_kind(AUTOMATIC_LIVELINESS_QOS)
            , liveliness_lease_duration(c_TimeInfinite)
            , mode(SYNCHRONOUS_WRITER)
            , disable_heartbeat_piggyback(false)
            , disable_positive_acks(false)
            , keep_duration(c_TimeInfinite)
        {
            endpoint.endpointKind = WRITER;
            endpoint.durabilityKind = TRANSIENT_LOCAL;
            endpoint.reliabilityKind = RELIABLE;
        }

        virtual ~WriterAttributes(){}

        //!Attributes of the associated endpoint.
        EndpointAttributes endpoint;

        //!Writer Times (only used for RELIABLE).
        WriterTimes times;

        //! Liveliness kind
        LivelinessQosPolicyKind liveliness_kind;

        //! Liveliness lease duration
        Duration_t liveliness_lease_duration;

        //!Indicates if the Writer is synchronous or asynchronous
        RTPSWriterPublishMode mode;

        // Throughput controller, always the last one to apply
        ThroughputControllerDescriptor throughputController;

        //! Disable the sending of heartbeat piggybacks.
        bool disable_heartbeat_piggyback;

        //! Define the allocation behaviour for matched-reader-dependent collections.
        ResourceLimitedContainerConfig matched_readers_allocation;

        //! Disable the sending of positive ACKs
        bool disable_positive_acks;

        //! Keep duration to keep a sample before considering it has been acked
        Duration_t keep_duration;
};

} /* namespace rtps */
} /* namespace fastrtps */
} /* namespace eprosima */

#endif /* WRITERATTRIBUTES_H_ */
