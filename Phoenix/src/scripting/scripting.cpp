#include "ph_pch.h"
#include "scripting.h"

#include <jxcore.h>

#include "src/ECS/components.h"
#include "src/Engine.h"

namespace Phoenix{

	//////////////////////////////////////////////////////////////////////
	// methods for inside scripting

	// void jx_setId(JXResult *results, int argc){
	// 	jxcore::scripting->setId(jxcore::getString(&results[0]));
	// }

	// void jx_restoreId(JXResult *results, int argc){
	// 	jxcore::scripting->restoreId();
	// }


	//////////////////////////////////////////////////////////////////////
	// transform
	void jx_getX(JXResult *results, int argc){
		jxcore::setDouble(&results[argc], jxcore::scripting->getX((uint32_t)jxcore::getInt(&results[0])));
	}
	void jx_setX(JXResult *results, int argc){

		jxcore::scripting->setX((uint32_t)jxcore::getInt(&results[0]), jxcore::getDouble(&results[1]));
	}

	// void jx_getY(JXResult *results, int argc){
	// 	jxcore::setDouble(&results[argc], jxcore::scripting->getY());
	// }
	// void jx_setY(JXResult *results, int argc){
	// 	jxcore::scripting->setY(jxcore::getDouble(&results[0]));
	// }

	// void jx_getZ(JXResult *results, int argc){
	// 	jxcore::setDouble(&results[argc], jxcore::scripting->getZ());
	// }
	// void jx_setZ(JXResult *results, int argc){
	// 	jxcore::scripting->setZ(jxcore::getDouble(&results[0]));
	// }



	//////////////////////////////////////////////////////////////////////
	// inputs
	void jx_keyDown(JXResult *results, int argc){
		jxcore::setBool(&results[argc], jxcore::scripting->keyDown(jxcore::getInt(&results[0])));
	}


	//////////////////////////////////////////////////////////////////////
	// engine

	void jx_frameTime(JXResult *results, int argc){
		jxcore::setDouble(&results[argc], jxcore::scripting->frameTime());
	}



	//////////////////////////////////////////////////////////////////////
	// scripting setup

	void Scripting::init(std::string path){
		jxcore::init(path.c_str());

		// jxcore::defineExtension("setId", "entity.setId", jx_setId);
		// jxcore::defineExtension("restoreId", "entity.restoreId", jx_restoreId);


		//////////////////////////////////////////////////////////////////////
		// transform
		jxcore::defineExtension("getX", jx_getX);
		jxcore::defineExtension("setX", jx_setX);

		// jxcore::defineExtension("getY", "entity.getY", jx_getY);
		// jxcore::defineExtension("setY", "entity.setY", jx_setY);

		// jxcore::defineExtension("getZ", "entity.getZ", jx_getZ);
		// jxcore::defineExtension("setZ", "entity.setZ", jx_setZ);


		//////////////////////////////////////////////////////////////////////
		// inputs
		jxcore::defineExtension("keyDown", "phoenix.keyDown", jx_keyDown);


		//////////////////////////////////////////////////////////////////////
		// engine
		jxcore::defineExtension("frameTime", "phoenix.frameTime", jx_frameTime);


	}

	void Scripting::close(){
		jxcore::close();
	}


	void Scripting::run(std::string code){
		jxcore::run(code);
	}


	void Scripting::createScript(std::string script, std::string script_path){
		jxcore::run("try{var script=new _Script();\n" + script + "\n_script_manager.create_script(\"" + script_path + "\",script);}catch(e){process.natives.warning('\\n', e.stack);}");
		//jxcore::run_script_controller(1, script_path);
	}


	void Scripting::scriptControllerCreate(std::string script_id, Entity entity, Engine* engine){
		jxcore::setScripting(this);
		_engine = engine;

		jxcore::run_script_controller(1, script_id, entity.getID());
	}


	void Scripting::scriptControllerUpdate(std::string script_id, Entity entity, Engine* engine){
		jxcore::setScripting(this);
		_engine = engine;

		jxcore::run_script_controller(2, script_id, entity.getID());
	}


	//////////////////////////////////////////////////////////////////////
	// methods for running by JX

	// void Scripting::setId(std::string uuid){
	// 	UUID id { (uint64_t)std::stoull(uuid) };
	// 	_entity = _engine->getScene()->getEntityByUUID(id);
	// }

	// void Scripting::restoreId(){
	// 	_entity = _entity_save;
	// }

	//////////////////////////////////////////////////////////////////////
	// transform
	double Scripting::getX(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().position.x;
	}
	void Scripting::setX(uint32_t id, double x){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().position.x = (float)x;
	}


	//////////////////////////////////////////////////////////////////////
	// inputs

	bool Scripting::keyDown(int key){
		return _engine->keyDown(key);
	}
	


	//////////////////////////////////////////////////////////////////////
	// engine

	double Scripting::frameTime(){
		return _engine->performanceMetrics.engineLoop / 1000;
	}



}
