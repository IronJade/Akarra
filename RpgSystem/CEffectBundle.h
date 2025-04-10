/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    "Bundle" is my way of telling it's a list of several items, stored to a file.
 *    Since I didn't know how hash- or treemaps worked back when I wrote these bundles,
 *    I'm looping to find a requested data object.
 **/

#ifndef _CEFFECTBUNDLE_H_
#define _CEFFECTBUNDLE_H_

#include <vector>

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{

	class CEffectType;

	// EFFECT TYPE BUNDLE ---------------------------------------------------------------------------

	enum EEffectBundleFileVersions
	{
		EBFV_ORIGINAL = 0,

		EBFV_CURRENT
	};

	typedef std::vector<CEffectType*>		TDEffectTypeVec;

	class CEffectTypeBundle
	{
		public:

			CEffectTypeBundle();
			~CEffectTypeBundle();

			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);
			bool open(char* filename);
			bool save(char* filename);

			// types

			TDEffectTypeVec& getEffectTypes();
			int getNumEffectTypes();

			CEffectType* getEffectType(char* dataname);

			CEffectType* createNewEffectType();
			bool addEffectType(CEffectType* item);
			bool deleteEffectType(char* name);

		protected:

			void m_clear();

		protected:

			TDEffectTypeVec		m_types;

	};

};

#endif
