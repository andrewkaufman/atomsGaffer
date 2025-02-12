//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2018, Toolchefs Ltd. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      * Redistributions of source code must retain the above
//        copyright notice, this list of conditions and the following
//        disclaimer.
//
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following
//        disclaimer in the documentation and/or other materials provided with
//        the distribution.
//
//      * Neither the name of John Haddon nor the names of
//        any other contributors to this software may be used to endorse or
//        promote products derived from this software without specific prior
//        written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef ATOMSGAFFER_ATOMSCROWDGENERATOR_H
#define ATOMSGAFFER_ATOMSCROWDGENERATOR_H

#include "AtomsGaffer/TypeIds.h"

#include "GafferScene/BranchCreator.h"

#include "IECoreScene/MeshPrimitive.h"

#include "Gaffer/PlugType.h"
#include "Gaffer/StringPlug.h"

namespace AtomsGaffer
{

class AtomsCrowdGenerator : public GafferScene::BranchCreator
{

	public:

		AtomsCrowdGenerator( const std::string &name = defaultName<AtomsCrowdGenerator>() );
		~AtomsCrowdGenerator() = default;

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( AtomsGaffer::AtomsCrowdGenerator, TypeId::AtomsCrowdGeneratorTypeId, GafferScene::BranchCreator );

		Gaffer::StringPlug *namePlug();
		const Gaffer::StringPlug *namePlug() const;

		GafferScene::ScenePlug *variationsPlug();
		const GafferScene::ScenePlug *variationsPlug() const;

		Gaffer::BoolPlug *useInstancesPlug();
		const Gaffer::BoolPlug *useInstancesPlug() const;

        Gaffer::FloatPlug *boundingBoxPaddingPlug();
        const Gaffer::FloatPlug *boundingBoxPaddingPlug() const;

        GafferScene::ScenePlug *clothCachePlug();
        const GafferScene::ScenePlug *clothCachePlug() const;

		void affects( const Gaffer::Plug *input, AffectedPlugsContainer &outputs ) const override;

	protected:

		void hash( const Gaffer::ValuePlug *output, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		void compute( Gaffer::ValuePlug *output, const Gaffer::Context *context ) const override;

		void hashBranchBound( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		Imath::Box3f computeBranchBound( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const override;

		void hashBranchTransform( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		Imath::M44f computeBranchTransform( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const override;

		void hashBranchAttributes( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		IECore::ConstCompoundObjectPtr computeBranchAttributes( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const override;

		void hashBranchObject( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		IECore::ConstObjectPtr computeBranchObject( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const override;

		void hashBranchChildNames( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		IECore::ConstInternedStringVectorDataPtr computeBranchChildNames( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const override;

		void hashBranchSetNames( const ScenePath &parentPath, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		IECore::ConstInternedStringVectorDataPtr computeBranchSetNames( const ScenePath &parentPath, const Gaffer::Context *context ) const override;

		void hashBranchSet( const ScenePath &parentPath, const IECore::InternedString &setName, const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		IECore::ConstPathMatcherDataPtr computeBranchSet( const ScenePath &parentPath, const IECore::InternedString &setName, const Gaffer::Context *context ) const override;

	private :

		Gaffer::AtomicCompoundDataPlug *agentChildNamesPlug();
		const Gaffer::AtomicCompoundDataPlug *agentChildNamesPlug() const;

		IECore::ConstCompoundDataPtr agentChildNames( const ScenePath &parentPath, const Gaffer::Context *context ) const;
		void agentChildNamesHash( const ScenePath &parentPath, const Gaffer::Context *context, IECore::MurmurHash &h ) const;

		void atomsPoseHash( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context, IECore::MurmurHash &h) const;

        IECore::ConstCompoundDataPtr agentCacheData(const ScenePath &branchPath) const;

        IECore::ConstCompoundDataPtr agentClothMeshData( const ScenePath &parentPath, const ScenePath &branchPath ) const;

        Imath::Box3d agentClothBoudingBox( const ScenePath &parentPath, const ScenePath &branchPath ) const;

        void applySkinDeformer(
                const ScenePath &branchPath,
                IECoreScene::MeshPrimitivePtr& result,
                IECoreScene::ConstMeshPrimitivePtr& meshPrim,
                IECore::ConstCompoundObjectPtr& meshAttributes,
                const std::vector<Imath::M44d>& worldMatrices
        		) const;

        void applyBlendShapesDeformer(
                const ScenePath &branchPath,
                IECoreScene::MeshPrimitivePtr& result,
                const IECore::CompoundData* metadataData,
                const IECore::CompoundDataMap& pointVariablesData,
                const int agentIdPointIndex
        		) const;

		bool applyClothDeformer(
				const ScenePath &branchPath,
				IECoreScene::MeshPrimitivePtr& result,
				IECore::ConstCompoundDataPtr& cloth,
				const Imath::M44f rootMatrix
				) const;

        Imath::M44f agentRootMatrix( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const;

		struct AgentScope : public Gaffer::Context::EditableScope
		{
			AgentScope( const Gaffer::Context *context, const ScenePath &branchPath );
		};

		static size_t g_firstPlugIndex;

};

} // namespace AtomsGaffer

#endif // ATOMSGAFFER_ATOMSCROWDGENERATOR_H
