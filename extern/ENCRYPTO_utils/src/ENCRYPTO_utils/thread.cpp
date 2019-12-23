#include "thread.h"
#include <cassert>
#include <condition_variable>
#include <mutex>
#include <thread>

double CEvent::maxWaitTime = 0;

CThread::CThread() : m_bRunning(false) {
}
CThread::~CThread() {
	assert(!m_bRunning);
}

bool CThread::Start() {
	thread_ = std::thread([this] { ThreadMain(); });
	m_bRunning = true;
	return true;
}

bool CThread::Wait() {
	if (!m_bRunning)
		return true;
	m_bRunning = false;
	thread_.join();
	return true;
}

bool CThread::IsRunning() const {
	return m_bRunning;
}

void CLock::Lock() {
	mutex_.lock();
}
void CLock::Unlock() {
	mutex_.unlock();
}

void CLock::lock() {
	Lock();
}
void CLock::unlock() {
	Unlock();
}


CEvent::CEvent(bool bManualReset, bool bInitialSet)
: m_bManual(bManualReset), m_bSet(bInitialSet)
{
}

bool CEvent::Set() {
	std::unique_lock<std::mutex> lock(mutex_);
	if (m_bSet)
		return true;

	m_bSet = true;
	lock.unlock();
	cv_.notify_one();
	return true;
}

bool CEvent::Wait() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]{ return m_bSet; });

    if (!m_bManual)
        m_bSet = false;
    return true;
}

bool CEvent::WaitFor() {

	std::unique_lock<std::mutex> lock(mutex_);

    double tstart1, tend1, total = 0;
    tstart1 = clock();
    cv_.wait_for(lock, std::chrono::seconds(300), [this]{return m_bSet;});

	cv_.wait_for(lock, std::chrono::seconds(300), [this]{return m_bSet;});

    int i = 0;
    if((!m_bSet) && (i < 5)){
        printf("--- --- --- 1024 wait_for loop: %d\n", i++);

        tend1 = clock();
        total = (tend1-tstart1)/CLOCKS_PER_SEC;

        if(total > maxWaitTime){
            maxWaitTime = total;
            printf("--- --- --- 1024 in loop wait_for time(s): %f, loop = %d\n", total, i);
        }
        cv_.wait_for(lock, std::chrono::seconds(300), [this]{return m_bSet;});
    }

	if (!m_bManual)
		m_bSet = false;

    if(total > maxWaitTime){
        maxWaitTime = total;
        printf("--- --- --- 1024 wait_for time(s): %f, loop time: %d, set new maxWaitTime\n", total, i);
    }

    if(total > 250){
        printf("--- --- --- 0930 wait_for time(s): %f, loop time: %d\n", total, i);
	}
	return true;
}

bool CEvent::IsSet() const {
	std::lock_guard<std::mutex> lock(mutex_);
	return m_bSet;
}

bool CEvent::Reset() {
	std::lock_guard<std::mutex> lock(mutex_);
	m_bSet = false;
	return true;
}
