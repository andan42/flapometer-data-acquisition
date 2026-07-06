#include "flapoRawDataQueue.h"
#include "flapoData/flapoRawData/flapoRawDataPollCancelled/flapoRawDataPollCancelled.h"

#include <thread>
#include <mutex>

#include <gtest/gtest.h>


//test fixture
class FlapoRawDataQueueTest : public ::testing::Test
{
protected:
    FlapoRawDataQueue *queue;

    virtual void SetUp()
    {
        queue = new FlapoRawDataQueue();
    }

    virtual void TearDown()
    {
        delete queue;
    }
};

class FlapoRawDataTest : public FlapoRawData //allow us to test the abstract class
{
public:
    FlapoRawDataTest() : FlapoRawData(nullptr) {}
    ~FlapoRawDataTest() override {}
};

//test popCancelleable with data

TEST_F(FlapoRawDataQueueTest, pushAndPop)
{
    FlapoRawData *data = new FlapoRawDataTest();
    queue->push(data);
    bool active = true;
    FlapoRawData *popped = queue->popCancelleable(&active);
    ASSERT_EQ(data, popped);
    delete popped;
}

//test pushing into a waiting queue
TEST_F(FlapoRawDataQueueTest, popAndPush)
{
    FlapoRawData *data = new FlapoRawDataTest();
    std::thread t([this, data] {
        bool active = true;
        FlapoRawData *popped = queue->popCancelleable(&active);
        ASSERT_EQ(data, popped);
        delete popped;
    });
    queue->push(data);
    t.join();
}

//test cancelling pop
TEST_F(FlapoRawDataQueueTest, popCancelleableCancelled)
{
    bool active = true;
    std::thread t([this, &active] { //pass active by reference, so we can change it from the thread. otherwise the compiler treats it as a const. i expected this to copy the variable (which would also be bad), but it seems to be a reference
        FlapoRawData *popped = queue->popCancelleable(&active);
        FlapoRawDataPollCancelled *cancelled = dynamic_cast<FlapoRawDataPollCancelled *>(popped);
        ASSERT_NE(cancelled, nullptr);
        ASSERT_TRUE(cancelled->messageIs("FlapoRawDataQueue killed or popCancelleable() inactivated."));
        delete popped;
    });
    active = false;
    queue->notifyAll(); //notify is needed to wake up the thread, otherwise it iwll not check the active variable
    t.join();
}