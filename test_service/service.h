#ifndef _ZKS_SERVICE_H
#define _ZKS_SERVICE_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
//#include <any>

namespace zks {

namespace pattern {


    class Subscriber;

    class Service {
    public:
        using SubscriberPtr = std::weak_ptr<Subscriber>;
        using SubscriberCallBack = std::function<void(void*)>;
        //using SubscriberCallBack = std::any;
        
    private:
        std::unordered_map<SubscriberPtr, SubscriberCallBack> subscriptions_;

    public:
        Service();
        virtual ~Service() = 0;

        int reg(SubscriberPtr pSub, SubscriberCallBack cb);
        int unreg(SubscriberPtr pSub);
        int purge();
        int notify(void* msg = nullptr) const;
    };

    class Subscriber {
    public:
        using ServicePtr = std::weak_ptr<Service>;

        Subscriber();
        virtual ~Subscriber();

        int subscribe(ServicePtr svc) const;
        int unsubscribe(ServicePtr svc) const;

        virtual void on_notification(void* msg) = 0;
    };
}


}


#endif // _ZKS_SERVICE_H