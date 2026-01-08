#include <gtest/gtest.h>
#include "../Common.h"
#include <shared_mutex>
#include <map>

class MockStore {
public:
    std::map<int, std::shared_mutex> mutexes;
    int write_count = 0;

    void process(RequestType type, int id) {
        if (type == RequestType::WRITE_START) {
            mutexes[id].lock();
            write_count++;
        } else if (type == RequestType::WRITE_END) {
            mutexes[id].unlock();
        }
    }
};

TEST(LogicTest, WriteLockPreventsConcurrentAccess) {
    MockStore store;
    store.process(RequestType::WRITE_START, 101);
    
    bool locked = store.mutexes[101].try_lock();
    EXPECT_FALSE(locked);
    
    store.process(RequestType::WRITE_END, 101);
    locked = store.mutexes[101].try_lock();
    EXPECT_TRUE(locked);
    store.mutexes[101].unlock();
}

TEST(LogicTest, ReadLockAllowsMultipleReaders) {
    std::shared_mutex m;
    m.lock_shared();
    bool second_read = m.try_lock_shared();
    EXPECT_TRUE(second_read);
    
    if(second_read) m.unlock_shared();
    m.unlock_shared();
}