#pragma once

#include "ph_pch.h"

namespace Phoenix{
	class Timer{
		private:
			std::chrono::time_point<std::chrono::steady_clock> start_time;

		public:
			Timer(){
				start_time = std::chrono::high_resolution_clock::now();
			}

			~Timer(){}


			long long stop(){
				std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::high_resolution_clock::now();

				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

				return ((end-start));
			}
	};


	template<typename Fn>
	class ScopeTimer{
		private:
			std::chrono::time_point<std::chrono::steady_clock> start_time;
			Fn callback;
			
		public:
			ScopeTimer(Fn&& _callback)
				: callback(_callback){

				start_time = std::chrono::high_resolution_clock::now();
			}

			~ScopeTimer(){
				std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::high_resolution_clock::now();

				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

				callback((end-start) *  0.001f);
			}
	};


}

