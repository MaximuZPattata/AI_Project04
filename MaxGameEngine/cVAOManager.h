#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

#include <string>
#include <map>

#include "sModelDrawInfo.h"

class cVAOManager
{
public:
	bool LoadModelIntoVAO(std::string friendlyName, std::string fileName, sModelDrawInfo& drawInfo, unsigned int shaderProgramID, 
							bool bIsDynamicBuffer = false);

	bool FindDrawInfoByModelName(std::string filename,
		sModelDrawInfo& drawInfo);

	//std::string getLastError(bool bAndClear = true);

	void setBasePath(std::string basePathWithoutSlash);

	std::string getBasePath();

	void LoadModelIntoVAOWithExistingModelPath(std::string friendlyName, std::string fileName, sModelDrawInfo& drawInfo, unsigned int shaderProgramID,
		bool bIsDynamicBuffer = false);

private:
	/*bool m_LoadTheFile_Ply_XYZ_N_RGBA(std::string theFileName, sModelDrawInfo& drawInfo);

	bool m_LoadTheFile_Ply_XYZ_N_RGBA_UV(std::string theFileName, sModelDrawInfo& drawInfo);*/

	bool m_LoadTheFile(std::string fileName, sModelDrawInfo& drawInfo);

	std::map< std::string, sModelDrawInfo> m_map_ModelName_to_VAOID;

	std::string m_basePathWithoutSlash;
};

#endif
