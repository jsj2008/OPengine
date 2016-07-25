#include "./Human/include/Platform/opengl/OPindexBufferGL.h"
#include "./Human/include/Rendering/OPindexBuffer.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPmemory.h"

OPindexBuffer* OPindexBufferGLInit(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGL* internalPtr = (OPindexBufferAPIGL*)OPalloc(sizeof(OPindexBufferAPIGL));;
	indexBuffer->internalPointer = internalPtr;
	OPGLFN(glGenBuffers(1, &internalPtr->Handle));
	return indexBuffer;
}

OPindexBuffer* OPindexBufferGLCreate() {
	OPindexBuffer* indexBuffer = (OPindexBuffer*)OPalloc(sizeof(OPindexBuffer));
	return OPindexBufferGLInit(indexBuffer);
}

void OPindexBufferGLSetData(OPindexBuffer* indexBuffer, OPindexSize size, OPuint count, const void* data) {
	OPindexBufferAPIGL* buffer = (OPindexBufferAPIGL*)indexBuffer->internalPointer;
	indexBuffer->ElementSize = size;
	indexBuffer->ElementCount = count;
	ui32 totalSize = (ui32)indexBuffer->ElementSize * count;
	OPGLFN(glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalSize, data, GL_STATIC_DRAW));
}

void OPindexBufferGLBind(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGL* buffer = (OPindexBufferAPIGL*)indexBuffer->internalPointer;
	OPGLFN(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->Handle));
}

void OPindexBufferGLUnbind(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGL* buffer = (OPindexBufferAPIGL*)indexBuffer->internalPointer;
}

void OPindexBufferGLDestroy(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGL* buffer = (OPindexBufferAPIGL*)indexBuffer->internalPointer;
	OPGLFN(glDeleteBuffers(1, &buffer->Handle));
}

void OPindexBufferAPIGLInit(OPindexBufferAPI* indexBuffer) {
	indexBuffer->Init = OPindexBufferGLInit;
	indexBuffer->Create = OPindexBufferGLCreate;
	indexBuffer->SetData = OPindexBufferGLSetData;
	indexBuffer->Bind = OPindexBufferGLBind;
	indexBuffer->Unbind = OPindexBufferGLUnbind;
	indexBuffer->Destroy = OPindexBufferGLDestroy;
}