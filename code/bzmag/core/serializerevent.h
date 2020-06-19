#ifndef BZMAG_CORE_SERIALIZER_SERIALIZEREVENT_H
#define BZMAG_CORE_SERIALIZER_SERIALIZEREVENT_H
/**
    @ingroup bzmagCoreSerializer
    @class bzmag::SerializerEvent
    @brief
*/

namespace bzmag
{
    class SerializerEvent
    {
    public:
        virtual void beginSerialize() {}
        virtual void endSerialize() {}

        virtual void beginDeserialize() {}
        virtual void endDeserialize() {}
    };
}

#endif // BZMAG_CORE_SERIALIZER_SERIALIZEREVENT_H
