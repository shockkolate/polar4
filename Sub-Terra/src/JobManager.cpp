#include "common.h"
#include "JobManager.h"

/* prevent macros from overriding standard functions */
#ifdef max
#undef max
#endif

JobManager::JobManager(Polar *engine) : System(engine) {
	for(int i = 0; i < numWorkers; ++i) {
		_workers.push_back(new Worker());
	}
}

JobManager::~JobManager() {
	for(auto worker : _workers) {
		delete worker;
	}
}

void JobManager::Init() {
	for(auto worker : _workers) {
		worker->Start();
	}
}

void JobManager::Update(DeltaTicks &, std::vector<Object *> &) {
	auto numJobs = jobs.With<JobsType::size_type>([] (JobsType &jobs) { return jobs.size(); });
	if(numJobs == 0) {
		std::this_thread::yield();
		return;
	}

	auto numCycles = numWorkers * std::max(1U, numJobs / 48);
	for(std::vector<Worker *>::size_type i = 0; i < numCycles; ++i) {
		if(jobs.With<bool>([] (JobsType &jobs) { return jobs.empty(); })) {
			std::this_thread::yield();
			break;
		} else {
			auto job = jobs.With<Job>([] (JobsType &jobs) {
				auto job = jobs.top();
				jobs.pop();
				return job;
			});
			switch(job.thread) {
			case JobThread::Main:
				job.fn();
				break;
			case JobThread::Worker:
				if(nextWorker >= _workers.size()) { nextWorker = 0; }
				_workers.at(nextWorker++)->AddJob(job);
				std::this_thread::yield();
				break;
			case JobThread::Any:
				if(nextWorker == _workers.size()) {
					job.fn();
				} else {
					if(nextWorker > _workers.size()) { nextWorker = 0; }
					_workers.at(nextWorker++)->AddJob(job);
					std::this_thread::yield();
				}
				break;
			}
		}
	}
}

void JobManager::Destroy() {
	for(auto worker : _workers) {
		worker->AddJob(Job(JobType::Stop));
	}
	for(auto worker : _workers) {
		worker->Join();
	}
}
