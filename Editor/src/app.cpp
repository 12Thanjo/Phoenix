#include <Phoenix.h>


class Editor : public Phoenix::Application{
	private:

	public:
		Editor(){

		}	

		~Editor(){
			
		}

};



Phoenix::Application* Phoenix::initialize(){
	return new Editor();
}