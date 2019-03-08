#pragma once

namespace prev {

	class InstancedBuffer {
	public:
		inline void SetNumerOfInstances(unsigned int numInstances) { m_NumberInstance = numInstances; }
		inline unsigned int GetNumberOfInstances() const { return m_NumberInstance; }
		inline unsigned int GetDataSize() const { return m_DataSize; }
	public:
		virtual void AppendData(unsigned long long int dataSize, const void * data) = 0;
		//Every Size is in bytes (even offset)
		virtual void ReplaceData(unsigned long long int dataOffet, unsigned long long dataSize, const void * data) = 0;
	protected:
		unsigned int m_NumberInstance = 0;
		unsigned int m_UpdateInterval = 1;
		unsigned int m_CoordinateSize = 2;
		unsigned long long int m_DataSize = 0;
		unsigned long long int m_MaxDataSize = 0;
	public:
		static InstancedBuffer * Create(unsigned long long maxDataSize);
	};

}