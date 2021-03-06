#include "./Performance/include/OPphysX.h"

#ifdef OPIFEX_OPTION_PHYSX

#include "./Performance/include/OPphysXBasicFilterShader.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"
#include "physxvisualdebuggersdk/PvdConnection.h"
#include "./Performance/include/OPphysXController.h"


PxPhysics* OPphysXSDK = NULL;
PxCooking* OPphysXCooking = NULL;
PxFoundation* OPphysXFoundation = NULL;
static PxDefaultErrorCallback _defaultErrorCallback;
static PxDefaultAllocator _defaultAllocatorCallback;
PxSimulationFilterShader OPphysXDefaultFilterShader = OPphysXBasicFilterShader;

void OPphysXInit() {
	if(OPphysXSDK != NULL) return;

	OPphysXFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, _defaultAllocatorCallback, _defaultErrorCallback);
	OPphysXSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *OPphysXFoundation, PxTolerancesScale());
	PxInitExtensions(*OPphysXSDK);
	OPphysXCooking = PxCreateCooking(PX_PHYSICS_VERSION, *OPphysXFoundation, PxCookingParams(PxTolerancesScale()));
	if (!OPphysXCooking) {
		ASSERT(false, "Failed to initialize Cooking");
	}
}

void OPphysXDebugger(const OPchar* host) {

	// check if PvdConnection manager is available on this platform
	if(OPphysXSDK->getPvdConnectionManager() == NULL) {
		OPlog("No Debugger Support");
	    return;
	}

	// setup connection parameters
	// const char*     pvd_host_ip = "10.211.55.7";  // IP of the PC which is running PVD
	int             port        = 5425;         // TCP port to connect to, where PVD is listening
	unsigned int    timeout     = 100;          // timeout in milliseconds to wait for PVD to respond,
	                                            // consoles and remote PCs need a higher timeout.
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	// and now try to connect
	physx::debugger::comm::PvdConnection* theConnection = PxVisualDebuggerExt::createConnection(OPphysXSDK->getPvdConnectionManager(),
		host, port, timeout, connectionFlags);

	// // remember to release the connection at shutdown
	//     if (theConnection)
	//             theConnection->release();
}

PxTriangleMesh* OPphysXCreateTriangleMesh(ui32 vertCount, PxVec3* verts, ui32 vertStride, ui32 triCount, PxU32* indices, ui32 indStride) {
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count           = vertCount;
	meshDesc.points.stride          = vertStride;
	meshDesc.points.data            = verts;

	meshDesc.triangles.count        = triCount;
	meshDesc.triangles.stride       = 3*indStride;
	meshDesc.triangles.data         = indices;
	//meshDesc.flags = PxMeshFlag::eFLIPNORMALS;

	PxDefaultMemoryOutputStream writeBuffer;
	bool status = OPphysXCooking->cookTriangleMesh(meshDesc, writeBuffer);
	if(!status) {
		OPlogErr("Failed to create cooked triangle");
	    return NULL;
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	return OPphysXSDK->createTriangleMesh(readBuffer);
}

PxTriangleMesh* OPphysXCreateTriangleMesh(ui32 vertCount, PxVec3* verts, ui32 triCount, PxU32* indices) {
	return OPphysXCreateTriangleMesh(vertCount, verts, sizeof(PxVec3), triCount, indices, sizeof(PxU32));
}

void OPphysXSetFilter(OPphysXRigidActor* actor, ui32 filterGroup, ui32 filterMask) {
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)OPalloc(sizeof(PxShape*)*numShapes);
	actor->getShapes(shapes, numShapes);
	for(PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	OPfree(shapes);
}

OPphysXMaterial* OPphysXCreateMaterial(f32 staticFriction, f32 dynamicFriction, f32 restitution) {
	OPphysXMaterial* material = OPphysXSDK->createMaterial(staticFriction, dynamicFriction, restitution);
	return material;
}

OPphysXShape* OPphysXAddSphereShape(OPphysXRigidActor* actor, OPphysXMaterial* material, OPfloat size) {
	PxShape* sphereShape = actor->createShape(PxSphereGeometry(size), *material);
	return sphereShape;
}

OPphysXShape* OPphysXAddBoxShape(OPphysXRigidActor* actor, OPphysXMaterial* material, OPvec3 size) {
	PxShape* boxShape = actor->createShape(PxBoxGeometry(size.x, size.y, size.z), *material);
	return boxShape;
}

OPphysXShape* OPphysXAddPlaneShape(OPphysXRigidActor* actor, OPphysXMaterial* material) {
	//PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxShape* planeShape = actor->createShape(PxPlaneGeometry(), *material);
	return planeShape;
}

OPphysXShape* OPphysXAddTriangleMeshShape(OPphysXRigidActor* actor, OPphysXMaterial* material, PxTriangleMesh* mesh) {
	PxShape* triMeshShape = actor->createShape(PxTriangleMeshGeometry(mesh), *material);
	return triMeshShape;
}

void OPphysXToMat4(PxTransform pT, OPmat4* mat) {

	PxMat33 m = PxMat33(pT.q);

	mat->cols[0].x = m.column0[0];
	mat->cols[0].y = m.column0[1];
	mat->cols[0].z = m.column0[2];
	mat->cols[0].w = 0;

	mat->cols[1].x = m.column1[0];
	mat->cols[1].y = m.column1[1];
	mat->cols[1].z = m.column1[2];
	mat->cols[1].w = 0;

	mat->cols[2].x = m.column2[0];
	mat->cols[2].y = m.column2[1];
	mat->cols[2].z = m.column2[2];
	mat->cols[2].w = 0;

	mat->cols[3].x = pT.p[0];
	mat->cols[3].y = pT.p[1];
	mat->cols[3].z = pT.p[2];
	mat->cols[3].w = 1;
}

PxTransform OPphysXMat4ToPx(OPmat4* mat) {

	PxTransform result = PxTransform(PxMat44((f32*)mat));
	return result;
}

void OPphysXShapeSetPose(OPphysXShape* shape, OPmat4 transform) {
	shape->setLocalPose(OPphysXMat4ToPx(&transform));
}

// Actor is a box with 2 shapes:
//		A Box Geometry that has Simulation turned on
//		A Box Geometry twice the size that has Simulation turned off and Triggers turned on
// Other is Capsule PxController with only the default shape

i8 OPphysXOverlapping(OPphysXRigidActor* actor, PxGeometry* otherGeometry, PxTransform otherTransform) {
	ui32 actorShapeCount = actor->getNbShapes();
	PxShape** actorShapes = new PxShape*[actorShapeCount];
	actor->getShapes(actorShapes, actorShapeCount);

	i8 collisionFound = 0;

	for(OPuint i = 0; i < actorShapeCount; i++) {
		if(PxShapeExt::overlap(*actorShapes[i], *actor, *otherGeometry, otherTransform)) {
			collisionFound = 1;
		}
	}

	OPfree(actorShapes);

	return collisionFound;
}

i8 OPphysXShapeOverlapping(OPphysXRigidActor* actor, OPphysXShape* shape, PxGeometry* otherGeometry, PxTransform otherTransform) {
	return PxShapeExt::overlap(*shape, *actor, *otherGeometry, otherTransform);
}

i8 OPphysXShapeOverlapping(PxTransform transform, OPphysXShape* shape, PxGeometry* otherGeometry, PxTransform otherTransform) {
		return PxGeometryQuery::overlap(shape->getGeometry().any(), transform, *otherGeometry, otherTransform);
}

i8 OPphysXOverlapping(OPphysXRigidActor* actor, OPphysXRigidActor* other) {
	ui32 actorShapeCount = actor->getNbShapes();
	ui32 otherShapeCount = other->getNbShapes();

	if(actorShapeCount <= 0 || otherShapeCount <= 0) return 0;

	PxShape** actorShapes = new PxShape*[actorShapeCount];
	PxShape** otherShapes = new PxShape*[otherShapeCount];

	actor->getShapes(actorShapes, actorShapeCount);
	other->getShapes(otherShapes, otherShapeCount);

	i8 collisionFound = 0;

	for(OPuint i = 0; i < actorShapeCount; i++) {
		for(OPuint j = 0; j < otherShapeCount; j++) {
			PxGeometry otherGeometry = otherShapes[j]->getGeometry().any();
			PxTransform otherTransform = PxShapeExt::getGlobalPose(*otherShapes[j], *other);
			if(PxShapeExt::overlap(*actorShapes[i], *actor, otherGeometry, otherTransform)) {
				collisionFound = 1;
				break;
			}
		}

		if(collisionFound) {
			break;
		}
	}

	OPfree(actorShapes);
	OPfree(otherShapes);

	return collisionFound;
}

PxGeometry GetGeoByType(PxGeometryHolder geo) {
	switch (geo.getType()) {
	case PxGeometryType::eCAPSULE: {
		return geo.capsule();
	}
	case PxGeometryType::eBOX: {
		return geo.box();
	}
	}
}

// Stupidly, this has an issue when it converts the geometry back to the types.
// Solution, for now, is to send the actual typed geometry to the overlap
//i8 OPphysXOverlapping(OPphysXRigidActor* actor, PxShape* actorShape, OPphysXRigidActor* other, PxShape* otherShape) {
//
//	PxGeometry actorGeometry = GetGeoByType(actorShape->getGeometry());
//	PxTransform actorTransform = PxShapeExt::getGlobalPose(*actorShape, *actor);
//
//	PxGeometry otherGeometry = GetGeoByType(otherShape->getGeometry());
//	PxTransform otherTransform = PxShapeExt::getGlobalPose(*otherShape, *other);
//
//	return PxGeometryQuery::overlap(actorGeometry, actorTransform, otherGeometry, otherTransform);
//}

i8 OPphysXOverlapping(OPphysXRigidActor* actor, PxShape* actorShape, OPphysXRigidActor* other, PxShape* otherShape) {

	const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(actorShape->getGeometry().capsule());
	const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(otherShape->getGeometry().box());

	//PxGeometry actorGeometry = GetGeoByType(actorShape->getGeometry());
	PxTransform actorTransform = PxShapeExt::getGlobalPose(*actorShape, *actor);

	//PxGeometry otherGeometry = GetGeoByType(otherShape->getGeometry());
	PxTransform otherTransform = PxShapeExt::getGlobalPose(*otherShape, *other);

	return PxGeometryQuery::overlap(capsuleGeom, actorTransform, boxGeom, otherTransform);
}

bool OPphysXOverlapping(OPphysXController* controller, PxShape* actorShape, OPphysXRigidActor* other, PxShape* otherShape) {
	const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(actorShape->getGeometry().capsule());
	const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(otherShape->getGeometry().box());

	PxTransform actorTransform = PxShapeExt::getGlobalPose(*actorShape, *controller->getActor());
	PxTransform otherTransform = PxShapeExt::getGlobalPose(*otherShape, *other);

	return PxGeometryQuery::overlap(capsuleGeom, actorTransform, boxGeom, otherTransform);
}

bool OPphysXOverlappingSphere(OPphysXController* controller, PxShape* actorShape, OPphysXRigidActor* other, PxShape* otherShape) {
	const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(actorShape->getGeometry().capsule());
	const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(otherShape->getGeometry().sphere());

	PxTransform actorTransform = PxShapeExt::getGlobalPose(*actorShape, *controller->getActor());
	PxTransform otherTransform = PxShapeExt::getGlobalPose(*otherShape, *other);

	return PxGeometryQuery::overlap(capsuleGeom, actorTransform, sphereGeom, otherTransform);
}


bool OPphysXOverlappingSphereSphere(OPphysXController* controller, PxShape* actorShape, OPphysXRigidActor* other, PxShape* otherShape) {
	const PxSphereGeometry& capsuleGeom = static_cast<const PxSphereGeometry&>(actorShape->getGeometry().sphere());
	const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(otherShape->getGeometry().sphere());

	PxTransform actorTransform = PxShapeExt::getGlobalPose(*actorShape, *controller->getActor());
	PxTransform otherTransform = PxShapeExt::getGlobalPose(*otherShape, *other);

	return PxGeometryQuery::overlap(capsuleGeom, actorTransform, sphereGeom, otherTransform);
}

bool OPphysXOverlappingSphereBox(OPphysXController* controller, PxShape* actorShape, OPphysXRigidActor* other, PxShape* otherShape) {
	const PxSphereGeometry& capsuleGeom = static_cast<const PxSphereGeometry&>(actorShape->getGeometry().sphere());
	const PxBoxGeometry& sphereGeom = static_cast<const PxBoxGeometry&>(otherShape->getGeometry().box());

	PxTransform actorTransform = PxShapeExt::getGlobalPose(*actorShape, *controller->getActor());
	PxTransform otherTransform = PxShapeExt::getGlobalPose(*otherShape, *other);

	return PxGeometryQuery::overlap(capsuleGeom, actorTransform, sphereGeom, otherTransform);
}



bool OPphysXOverlappingSphere(OPphysXController* controller, PxShape* actorShape, PxTransform other, PxSphereGeometry sphereGeom) {
	const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(actorShape->getGeometry().capsule());

	PxTransform actorTransform = PxShapeExt::getGlobalPose(*actorShape, *controller->getActor());

	return PxGeometryQuery::overlap(capsuleGeom, actorTransform, sphereGeom, other);
}

bool OPphysXOverlappingSphere(OPphysXController* controller, PxShape* actorShape, OPvec3 pos, OPfloat size) {
	return OPphysXOverlappingSphere(controller, actorShape, PxTransform(PxVec3(pos.x, pos.y, pos.z)), PxSphereGeometry(size));
}

void OPphysXGetTransform(OPphysXRigidActor* actor, OPmat4* mat) {
	ui32 n = actor->getNbShapes();
	PxShape** shapes = new PxShape*[n];

	actor->getShapes(shapes, n);

	PxTransform pT = PxShapeExt::getGlobalPose(*shapes[0], *actor);

	PxMat33 m = PxMat33(pT.q);

	mat->cols[0].x = m.column0[0];
	mat->cols[0].y = m.column0[1];
	mat->cols[0].z = m.column0[2];
	mat->cols[0].w = 0;

	mat->cols[1].x = m.column1[0];
	mat->cols[1].y = m.column1[1];
	mat->cols[1].z = m.column1[2];
	mat->cols[1].w = 0;

	mat->cols[2].x = m.column2[0];
	mat->cols[2].y = m.column2[1];
	mat->cols[2].z = m.column2[2];
	mat->cols[2].w = 0;

	mat->cols[3].x = pT.p[0];
	mat->cols[3].y = pT.p[1];
	mat->cols[3].z = pT.p[2];
	mat->cols[3].w = 1;

	OPfree(shapes);
}

void OPphysXShutdown() {
	OPphysXCooking->release();
	OPphysXCooking = NULL;
	OPphysXSDK->release();
	OPphysXSDK = NULL;
	PxCloseExtensions();
	OPphysXFoundation->release();
	OPphysXFoundation = NULL;
}

i8 OPphysXBoxColliding(PxRigidDynamic* actor, OPvec3 size, OPvec3 pos) {
		PxBoxGeometry otherGeometry = PxBoxGeometry(size.x, size.y, size.z);
		PxTransform otherTransform = PxTransform(PxVec3(pos.x, pos.y, pos.z));
		return OPphysXOverlapping(actor, &otherGeometry, otherTransform);
}

i8 OPphysXShapeBoxColliding(PxRigidDynamic* actor, OPphysXShape* shape, OPvec3 size, OPvec3 pos) {
		PxBoxGeometry otherGeometry = PxBoxGeometry(size.x, size.y, size.z);
		PxTransform otherTransform = PxTransform(PxVec3(pos.x, pos.y, pos.z));
		return OPphysXShapeOverlapping(actor, shape, &otherGeometry, otherTransform);
}

i8 OPphysXShapeBoxColliding(OPmat4 transform, OPphysXShape* shape, OPvec3 size, OPvec3 pos) {
		PxBoxGeometry otherGeometry = PxBoxGeometry(size.x, size.y, size.z);
		PxTransform otherTransform = PxTransform(PxVec3(pos.x, pos.y, pos.z));
		return OPphysXShapeOverlapping(OPphysXMat4ToPx(&transform), shape, &otherGeometry, otherTransform);
}

#endif
