#include"addrDecod.h"

VIP_ENCULONG intVal(VIP_ENCINT* ptr){
	VIP_ENCULONG out;
	
	out = reinterpret_cast<std::uintptr_t>(ptr);
	
	return out;
}

VIP_ENCINT* addrVal(VIP_ENCULONG num){
	
	//VIP_DEC() was used because casting an int to pointer in ENC mode is not yet implemented
	VIP_ENCINT * out = (VIP_ENCINT *)VIP_DEC(num);
	return out;
}

