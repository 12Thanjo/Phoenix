#pragma once

#include <xhash>

namespace Phoenix{
	
	class UUID{
		private:
			uint64_t _id;
	
		public:
			UUID();
			UUID(uint64_t id);
			// UUID(const UUID&) = default;


			operator uint64_t() const { return _id; }
			operator std::string() const { return std::to_string(_id); }
			bool operator==(const UUID& other){
				return _id == other._id;
			}
	};


	// using UUID = uint64_t;

	// UUID generateUUID();

}


namespace std{
	
	template<>
	struct hash<Phoenix::UUID>{
		std::size_t operator()(const Phoenix::UUID& uuid) const {
			return hash<uint64_t>()((uint64_t)uuid);
		};
	};

}
