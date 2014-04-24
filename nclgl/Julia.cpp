#include "Julia.h"

void JuliaSet::Draw() {
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, colorMap);
	glBindVertexArray(arrayObject);
	if(bufferObject[INDEX_BUFFER])	{
		glDrawElements(type,numIndices,GL_UNSIGNED_INT,0);
	} else {
		glDrawArrays(type,0,numVertices);
	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_1D,0);
}
