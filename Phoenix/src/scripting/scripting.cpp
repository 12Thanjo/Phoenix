#include "ph_pch.h"
#include "scripting.h"

#include <jxcore.h>

#include "src/ECS/components.h"
#include "src/Engine.h"

namespace Phoenix{

	//////////////////////////////////////////////////////////////////////
	// methods for inside scripting

	void jx_setId(JXResult *results, int argc){
		jxcore::scripting->setId(jxcore::getString(&results[0]));
	}

	void jx_restoreId(JXResult *results, int argc){
		jxcore::scripting->restoreId();
	}


	//////////////////////////////////////////////////////////////////////
	// transform
	void jx_getX(JXResult *results, int argc){
		jxcore::setDouble(&results[argc], jxcore::scripting->getX());
	}
	void jx_setX(JXResult *results, int argc){
		jxcore::scripting->setX(jxcore::getDouble(&results[0]));
	}

	void jx_getY(JXResult *results, int argc){
		jxcore::setDouble(&results[argc], jxcore::scripting->getY());
	}
	void jx_setY(JXResult *results, int argc){
		jxcore::scripting->setY(jxcore::getDouble(&results[0]));
	}

	void jx_getZ(JXResult *results, int argc){
		jxcore::setDouble(&results[argc], jxcore::scripting->getZ());
	}
	void jx_setZ(JXResult *results, int argc){
		jxcore::scripting->setZ(jxcore::getDouble(&results[0]));
	}



	//////////////////////////////////////////////////////////////////////
	// inputs
	void jx_keyDown(JXResult *results, int argc){
		jxcore::setBool(&results[argc], jxcore::scripting->keyDown(jxcore::getInt(&results[0])));
	}



	//////////////////////////////////////////////////////////////////////
	// scripting setup

	void Scripting::init(std::string path){
		jxcore::init(path.c_str());

		jxcore::defineExtension("setId", "entity.setId", jx_setId);
		jxcore::defineExtension("restoreId", "entity.restoreId", jx_restoreId);


		//////////////////////////////////////////////////////////////////////
		// transform
		jxcore::defineExtension("getX", "entity.getX", jx_getX);
		jxcore::defineExtension("setX", "entity.setX", jx_setX);

		jxcore::defineExtension("getY", "entity.getY", jx_getY);
		jxcore::defineExtension("setY", "entity.setY", jx_setY);

		jxcore::defineExtension("getZ", "entity.getZ", jx_getZ);
		jxcore::defineExtension("setZ", "entity.setZ", jx_setZ);


		//////////////////////////////////////////////////////////////////////
		// inputs
		jxcore::defineExtension("keyDown", "phoenix.keyDown", jx_keyDown);
	}

	void Scripting::close(){
		jxcore::close();
	}


	void Scripting::run(std::string code){
		jxcore::run(code);
	}


	void Scripting::scriptControllerUpdate(std::string script_id, Entity entity, Engine* engine){
		jxcore::setScripting(this);
		_entity = entity;
		_entity_save = entity;
		_engine = engine;

		jxcore::run_script_controller(2, script_id);
	}


	//////////////////////////////////////////////////////////////////////
	// methods for running by JX

	void Scripting::setId(std::string uuid){
		UUID id { (uint64_t)std::stoull(uuid) };
		_entity = _engine->getScene()->getEntityByUUID(id);
	}

	void Scripting::restoreId(){
		_entity = _entity_save;
	}

	//////////////////////////////////////////////////////////////////////
	// transform
	double Scripting::getX(){
		return (double)_entity.getComponent<Component::Transform>().position.x;
	}
	void Scripting::setX(double x){
		_entity.getComponent<Component::Transform>().position.x = (float)x;
	}

	double Scripting::getY(){
		return (double)_entity.getComponent<Component::Transform>().position.y;
	}
	void Scripting::setY(double y){
		_entity.getComponent<Component::Transform>().position.y = (float)y;
	}

	double Scripting::getZ(){
		return (double)_entity.getComponent<Component::Transform>().position.z;
	}
	void Scripting::setZ(double z){
		_entity.getComponent<Component::Transform>().position.z = (float)z;
	}

	//////////////////////////////////////////////////////////////////////
	// inputs

	bool Scripting::keyDown(int key){
		return _engine->keyDown(key);
	}
	
}
