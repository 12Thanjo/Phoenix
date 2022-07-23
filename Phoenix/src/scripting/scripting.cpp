#include "ph_pch.h"
#include "scripting.h"

#include <jxcore.h>

#include "src/ECS/components.h"
#include "src/Engine.h"



// makes writing code for integrating with JX easier
#define DOUBLE(i) jxcore::getDouble(&results[i])
#define INT(i) (uint32_t)jxcore::getInt(&results[0])

#define RETURN_DOUBLE(val) jxcore::setDouble(&results[argc], val);
#define RETURN_BOOL(val) jxcore::setBool(&results[argc], val);

namespace Phoenix{

	//////////////////////////////////////////////////////////////////////
	// transform
	void jx_getX(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getX(INT(0)));
	}
	void jx_setX(JXResult *results, int argc){
		jxcore::scripting->setX(INT(0), DOUBLE(1));
	}

	void jx_getY(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getY(INT(0)));
	}
	void jx_setY(JXResult *results, int argc){
		jxcore::scripting->setY(INT(0), DOUBLE(1));
	}

	void jx_getZ(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getZ(INT(0)));
	}
	void jx_setZ(JXResult *results, int argc){
		jxcore::scripting->setZ(INT(0), DOUBLE(1));
	}




	void jx_getRotationX(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getRotationX(INT(0)));
	}
	void jx_setRotationX(JXResult *results, int argc){
		jxcore::scripting->setRotationX(INT(0), DOUBLE(1));
	}

	void jx_getRotationY(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getRotationY(INT(0)));
	}
	void jx_setRotationY(JXResult *results, int argc){
		jxcore::scripting->setRotationY(INT(0), DOUBLE(1));
	}

	void jx_getRotationZ(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getRotationZ(INT(0)));
	}
	void jx_setRotationZ(JXResult *results, int argc){
		jxcore::scripting->setRotationZ(INT(0), DOUBLE(1));
	}




	void jx_getScaleX(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getScaleX(INT(0)));
	}
	void jx_setScaleX(JXResult *results, int argc){
		jxcore::scripting->setScaleX(INT(0), DOUBLE(1));
	}

	void jx_getScaleY(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getScaleY(INT(0)));
	}
	void jx_setScaleY(JXResult *results, int argc){
		jxcore::scripting->setScaleY(INT(0), DOUBLE(1));
	}

	void jx_getScaleZ(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->getScaleZ(INT(0)));
	}
	void jx_setScaleZ(JXResult *results, int argc){
		jxcore::scripting->setScaleZ(INT(0), DOUBLE(1));
	}

	//////////////////////////////////////////////////////////////////////
	// inputs
	void jx_keyDown(JXResult *results, int argc){
		RETURN_BOOL(jxcore::scripting->keyDown(INT(0)));
	}


	//////////////////////////////////////////////////////////////////////
	// engine

	void jx_frameTime(JXResult *results, int argc){
		RETURN_DOUBLE(jxcore::scripting->frameTime());
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

		jxcore::defineExtension("getY", jx_getY);
		jxcore::defineExtension("setY", jx_setY);

		jxcore::defineExtension("getZ", jx_getZ);
		jxcore::defineExtension("setZ", jx_setZ);


		jxcore::defineExtension("getRotationX", jx_getRotationX);
		jxcore::defineExtension("setRotationX", jx_setRotationX);

		jxcore::defineExtension("getRotationY", jx_getRotationY);
		jxcore::defineExtension("setRotationY", jx_setRotationY);

		jxcore::defineExtension("getRotationZ", jx_getRotationZ);
		jxcore::defineExtension("setRotationZ", jx_setRotationZ);


		jxcore::defineExtension("getScaleX", jx_getScaleX);
		jxcore::defineExtension("setScaleX", jx_setScaleX);

		jxcore::defineExtension("getScaleY", jx_getScaleY);
		jxcore::defineExtension("setScaleY", jx_setScaleY);

		jxcore::defineExtension("getScaleZ", jx_getScaleZ);
		jxcore::defineExtension("setScaleZ", jx_setScaleZ);


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


	void Scripting::updateInit(Engine* engine){
		_frame_time = _engine->performanceMetrics.engineLoop / 1000000;
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

	double Scripting::getY(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().position.y;
	}
	void Scripting::setY(uint32_t id, double y){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().position.y = (float)y;
	}


	double Scripting::getZ(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().position.z;
	}
	void Scripting::setZ(uint32_t id, double z){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().position.z = (float)z;
	}



	double Scripting::getRotationX(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().rotation.x;
	}
	void Scripting::setRotationX(uint32_t id, double x){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().rotation.x = (float)x;
	}

	double Scripting::getRotationY(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().rotation.y;
	}
	void Scripting::setRotationY(uint32_t id, double y){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().rotation.y = (float)y;
	}


	double Scripting::getRotationZ(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().rotation.z;
	}
	void Scripting::setRotationZ(uint32_t id, double z){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().rotation.z = (float)z;
	}



	double Scripting::getScaleX(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().scale.x;
	}
	void Scripting::setScaleX(uint32_t id, double x){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().scale.x = (float)x;
	}

	double Scripting::getScaleY(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().scale.y;
	}
	void Scripting::setScaleY(uint32_t id, double y){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().scale.y = (float)y;
	}


	double Scripting::getScaleZ(uint32_t id){
		return (double)Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().scale.z;
	}
	void Scripting::setScaleZ(uint32_t id, double z){
		Entity((entt::entity)id, _engine->getScene()).getComponent<Component::Transform>().scale.z = (float)z;
	}



	//////////////////////////////////////////////////////////////////////
	// inputs

	bool Scripting::keyDown(int key){
		return _engine->keyDown(key);
	}
	




}
