#include "service.h"
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <algorithm>
#include <unordered_map>

using namespace zks::pattern;
using namespace std;

class Goods : enable_shared_from_this<Goods> {
public:
    string name;
    int prod_time;
    enum State { INIT, PROD, DONE, CONSUMED };
    State state;
    Goods(string const& n, int t) : name(n), prod_time(t) { state = INIT; }
    ~Goods() = default;
};

using GoodsPtr = unique_ptr<Goods>;
using Gradient = unordered_map<string, size_t>;
using GoodsTray = unordered_map<string, vector<GoodsPtr>>;

class WareHouse : Service {
    string id_;
    size_t cap_;
    size_t size_;
    GoodsTray goods_map_;
    mutable recursive_mutex mutex_;

    void operator=(WareHouse const&) = delete;

    using lock_t = lock_guard<recursive_mutex>;
public:
    WareHouse(string const& id, size_t cap) : id_(id), cap_(cap), size_(0) {}
    ~WareHouse() {}

    string const& id() const { return id_; }
    size_t capacity() const { return cap_; }
    size_t idle_size() const {
        lock_t lock(mutex_);
        return cap_ - size_;
    }

    int add_goods(GoodsPtr&& g) {
        if (!g || g->state != Goods::State::DONE) {
            return -1;
        }
        lock_t lock(mutex_);
        if (idle_size() > 0) {
            goods_map_[g->name].emplace_back(move(g));
            return ++size_;
        }
        return -2;
    }

    size_t goods_count(string const& name) const {
        lock_t lock(mutex_);
        return goods_map_.count(name) ? goods_map_.at(name).size() : 0;
    }

    
};

class Factory : enable_shared_from_this<Factory> {
    mutable recursive_mutex mutex_;
    using lock_t = std::lock_guard<std::recursive_mutex>;
    string id_;
    vector<GoodsPtr> prod_queue;
public:
    Factory(string const& id, size_t s) : id_(id), prod_queue(s) {}
    string const& id() const { return id_; }
    size_t size() const {
        return prod_queue.size();
    }
    size_t idle_size() const {
        lock_t lock(mutex_);
        return count_if(prod_queue.cbegin(), prod_queue.cend(),
            logical_not<GoodsPtr>());
    }
    int add_goods(GoodsPtr&& p) {
        if (!p || (p->state != Goods::State::INIT)) {
            return -1;
        }
        lock_t lock(mutex_);
        int i = 0;
        for (;i < prod_queue.size();++i) {
            if (!prod_queue[i]) {
                prod_queue[i] = move(p);
                return i;
            }
        }
        return -2;
    }
};




int main() {
    return 0;
}