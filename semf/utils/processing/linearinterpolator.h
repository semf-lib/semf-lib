/**
 * @file linearinterpolator.h
 * @date 27.02.2019
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_PROCESSING_LINEARINTERPOLATOR_H_
#define SEMF_UTILS_PROCESSING_LINEARINTERPOLATOR_H_

#include <cstddef>
#include <cstdint>
#include <cmath>

namespace semf
{
/**
 * @brief Class for calculating the linear interpolation. It interpolates till 3-dimensional data.
 * @note The notation used in this class as follows: value as 'x' and the dimension's number is suffix,
 * i.e. \c x1 1st dimensional value, \c x2 2nd dimensional value. so a data point in 3d space (\c x1, \c x2, ,\c x3).
 * The corresponding value (target) of a given point is notated as \c y .
 *
 * @tparam MAX_DIMENTIONS Maximum dimensions for interpolation (1, 2, or 3).
 * @tparam T_IN Data type of inputs.
 * @tparam T_TARGET Data type of outputs.
 * @tparam T_STORAGE Data type of intermediate results.
 */
template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET = T_IN, typename T_STORAGE = T_TARGET>
class LinearInterpolator
{
public:
	/**
	 * Constructor.
	 * @param limitToBoundaryValues Flag that allows accepting input data beyond the boundaries, without throwing an error.
	 * @note \li \c true If a given data point is beyond the boundaries, then it will be replaced by the boundary point.
	 * 		 \li \c false If a given data point is beyond the boundaries, then \c m_retValue is \c false.
	 */
	explicit LinearInterpolator(bool limitToBoundaryValues = true);
	explicit LinearInterpolator(const LinearInterpolator& other) = delete;
	virtual ~LinearInterpolator() = default;

	/**
	 * @brief Initializes the class members with 1 dimensional data points.
	 * @param dimension1Data Pointer to the elements of the first dimension.
	 * @param targetData Pointer to the targetDataput dimension data.
	 * @param dimension1DataSize Number of elements of the first dimension.
	 * @param targetDataSize Number of elements of the targetDataput data.
	 * @return \c false if the number of output elements doesn't match the number of input elements,
	 * otherwise returns \c true.
	 */
	bool init(const T_IN dimension1Data[], uint16_t dimension1DataSize, const T_TARGET targetData[], uint16_t targetDataSize);

	/**
	 * @brief Initializes the class members with 2 dimensional data points.
	 * @param dimension1Data Pointer to the elements of the first dimension.
	 * @param dimension1DataSize Number of elements of the first dimension.
	 * @param dimension2Data Pointer to the elements of the second dimension.
	 * @param dimension2DataSize Number of elements of the second dimension.
	 * @param targetData Pointer to the targetDataput dimension data.
	 * @param targetDataSize Number of elements of the targetDataput data.
	 * @return \c false if the number of output elements doesn't match the number of input elements,
	 * otherwise returns \c true.
	 */
	bool init(const T_IN dimension1Data[], uint16_t dimension1DataSize, const T_IN dimension2Data[], uint16_t dimension2DataSize, const T_TARGET targetData[],
			  uint16_t targetDataSize);

	/**
	 * @brief Initializes the class members with 2 dimensional data points.
	 * @param dimension1Data Pointer to the elements of the first dimension.
	 * @param dimension1DataSize Number of elements of the first dimension.
	 * @param dimension2Data Pointer to the elements of the second dimension.
	 * @param dimension2DataSize Number of elements of the second dimension.
	 * @param dimension3Data Pointer to the elements of the third dimension.
	 * @param dimension3DataSize Number of elements of the third dimension.
	 * @param targetData Pointer to the targetDataput dimension data.
	 * @param targetDataSize Number of elements of the targetDataput data.
	 * @return \c false if the number of output elements doesn't match the number of input elements,
	 * otherwise returns \c true.
	 */
	bool init(const T_IN dimension1Data[], uint16_t dimension1DataSize, const T_IN dimension2Data[], uint16_t dimension2DataSize, const T_IN dimension3Data[],
			  uint16_t dimension3DataSize, const T_TARGET targetData[], uint16_t targetDataSize);

	/**
	 * @brief It updates/changes the \c m_targetDataSource array.
	 * @note On correct given parameters the \c m_targetDataSource array will be changed,
	 * otherwise \c m_targetDataSource will not be changed.
	 * @param targetData Pointer to the targetDataput dimension data.
	 * @param targetDataSize Number of elements of the targetDataput data.
	 * @return \c false if the number of output elements doesn't match the number of input elements,
	 * otherwise returns \c true.
	 */
	bool setTargetDataInputDimension(const T_TARGET targetData[], uint16_t targetDataSize);

	/**
	 * @brief Calculates the interpolation of a given data point.
	 * @param y Reference to the result (interpolated value).
	 * @param x1 Input data value for the first dimension.
	 * @param x2 Input data value for the second dimension.
	 * @param x3 Input data value for the third dimension.
	 * @return Success or failure of the interpolation. If the given data points are targetDataSource of given dimension boundaries.
	 * @return \li \c true on correct implementation,
	 * 		\li \c false if something goes wrong (e.g. the data point is beyond the boundary
	 * 		and \c m_limitToBoundaryValues is \c false).
	 */
	bool interpolate(T_TARGET& y, T_IN x1 = 0, T_IN x2 = 0, T_IN x3 = 0);

private:
	/**
	 * @brief Performs 1D interpolation.
	 */
	void interpolateInOneDimension();

	/**
	 * @brief Performs 2D interpolation.
	 */
	void interpolateInTwoDimensions();

	/**
	 * @brief Performs 3D interpolation.
	 */
	void interpolateInThreeDimensions();

	/**
	 * @brief Finds the vertices surrounding the input data point, and their corresponding outputs (targets).
	 */
	void calculateNeighboringPoints();

	/**
	 * @brief Finds the indices surrounding the input data point.
	 */
	void getPointIndecies();

	/**
	 * @brief Checks the indices surrounding the input data point whether it exceeds dimension's boundaries or not.
	 */
	void checkDimensionBoundaries();

	/**
	 * @brief Performs the absolute operation.
	 * @param input Input value.
	 * @return Absolute value of the input value.
	 */
	T_STORAGE abs(T_STORAGE);

	/** If input exceeds its boundary, algorithm considers the boundary value and throws no error.*/
	bool m_limitToBoundaryValues;
	/** In 1-dimensional is length, 2-dimensional is area, and 3-dimensional is volume bounded by the vertices.*/
	T_STORAGE m_partition = 0;
	/** Stores the pointers of the input dataset, elements of each dimension are stored in a separate array.*/
	const T_IN* m_dim[MAX_DIMENSIONS] = {nullptr};
	/** Pointer to a buffer that stores the given outputs (target values) of the input dataset.*/
	const T_TARGET* m_targetData = nullptr;
	/** It stores which dimension can be reduced during the interpolation,
	 * (i.e. 3D data but the interpolating is done on 2D only).*/
	bool m_dimReduction[MAX_DIMENSIONS] = {true};
	/** It counts the number of considered dimensions.*/
	uint8_t m_consideredDimCtr = 0;
	/** It stores number of elements/points in each dimension.*/
	uint16_t m_dimLen[MAX_DIMENSIONS] = {0};
	/** It stores number of elements/points of targetDataput data.*/
	uint16_t m_targetDataSize = 0;
	/** It stores the point indices, which are smaller in value than the
	 * given interpolating point for every dimension.*/
	int16_t m_iSVec[MAX_DIMENSIONS] = {-1};
	/** It stores the point indices, which are bigger in value than the given interpolating point for every dimension.*/
	int16_t m_iBVec[MAX_DIMENSIONS] = {-1};
	/** It stores the interpolating point for every dimension.*/
	T_IN m_xVal[MAX_DIMENSIONS] = {0};
	/** It stores the index of considered dimensions.*/
	uint8_t m_considDim[MAX_DIMENSIONS] = {0};
	/** It stores the output (target values) for the selected vertices.*/
	T_TARGET m_targetDataSel[8 /*2^3*/] = {0};
	T_TARGET m_yVal = 0; /**< It stores the result of the interpolation of a given point.*/
	/** The result of initialization or interpolation could be false
	 * (i.e. the given point is beyond the boundary.).*/
	bool m_retValue = false;
};

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::LinearInterpolator(bool limitToBoundaryValues)
: m_limitToBoundaryValues(limitToBoundaryValues)
{
	// Throws an error at compile time if maximal dimension is not in the allowed range (1, 2, or 3).
	static_assert(MAX_DIMENSIONS == 1 || MAX_DIMENSIONS == 2 || MAX_DIMENSIONS == 3, "MAX_DIMENSIONS has to be 1, 2 or 3");
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
bool LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::init(const T_IN dimension1Data[], uint16_t dimension1DataSize, const T_TARGET targetData[],
																		 uint16_t targetDataSize)
{
	if (dimension1Data == nullptr || targetData == nullptr || targetDataSize != dimension1DataSize)
	{
		m_retValue = false;
	}
	else
	{
		m_dim[0] = dimension1Data;
		m_dimLen[0] = dimension1DataSize;
		m_targetData = targetData;
		m_targetDataSize = targetDataSize;
		m_retValue = true;
	}
	return m_retValue;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
bool LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::init(const T_IN dimension1Data[], uint16_t dimension1DataSize, const T_IN dimension2Data[],
																		 uint16_t dimension2DataSize, const T_TARGET targetData[], uint16_t targetDataSize)
{
	if (dimension1Data == nullptr || dimension2Data == nullptr || targetData == nullptr || targetDataSize != (dimension1DataSize * dimension2DataSize))
	{
		m_retValue = false;
	}
	else
	{
		m_dim[0] = dimension1Data;
		m_dim[1] = dimension2Data;
		m_dimLen[0] = dimension1DataSize;
		m_dimLen[1] = dimension2DataSize;
		m_targetData = targetData;
		m_targetDataSize = targetDataSize;
		m_retValue = true;
	}
	return m_retValue;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
bool LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::init(const T_IN dimension1Data[], uint16_t dimension1DataSize, const T_IN dimension2Data[],
																		 uint16_t dimension2DataSize, const T_IN dimension3Data[], uint16_t dimension3DataSize,
																		 const T_TARGET targetData[], uint16_t targetDataSize)
{
	if (dimension1Data == nullptr || dimension2Data == nullptr || dimension3Data == nullptr || targetData == nullptr ||
		targetDataSize != (dimension1DataSize * dimension2DataSize * dimension3DataSize))
	{
		m_retValue = false;
	}
	else
	{
		m_dim[0] = dimension1Data;
		m_dim[1] = dimension2Data;
		m_dim[2] = dimension3Data;
		m_dimLen[0] = dimension1DataSize;
		m_dimLen[1] = dimension2DataSize;
		m_dimLen[2] = dimension3DataSize;
		m_targetData = targetData;
		m_targetDataSize = targetDataSize;
		m_retValue = true;
	}
	return m_retValue;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
bool LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::setTargetDataInputDimension(const T_TARGET targetData[], uint16_t targetDataSize)
{
	if (targetData == nullptr || targetDataSize != m_targetDataSize)
	{
		m_retValue = false;
	}
	else
	{
		m_targetData = targetData;
		m_retValue = true;
	}
	return m_retValue;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
bool LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::interpolate(T_TARGET& y, T_IN x1, T_IN x2, T_IN x3)
{
	m_retValue = true;
	m_yVal = 0;
	m_partition = 0;
	// reinitialize: m_dimReduction, m_iBVec, m_iSVec, m_consideredDimCtr
	for (uint8_t i = 0; i < MAX_DIMENSIONS; i++)
	{
		m_dimReduction[i] = false;
		m_iBVec[i] = 0;
		m_iSVec[i] = 0;
		m_considDim[i] = 0;
	}
	m_xVal[0] = x1;
	m_xVal[1] = x2;
	m_xVal[2] = x3;
	m_consideredDimCtr = 0;
	calculateNeighboringPoints();
	if (m_retValue)
	{
		switch (m_consideredDimCtr)
		{
			case 0:
				m_yVal = m_targetDataSel[0];
				break;
			case 1:
				interpolateInOneDimension();
				break;
			case 2:
				interpolateInTwoDimensions();
				break;
			case 3:
				interpolateInThreeDimensions();
				break;
			default:
				m_retValue = false;
				break;
		}
		y = m_yVal;
	}
	return m_retValue;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
void LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::interpolateInOneDimension()
{
	T_STORAGE nB = 0, nS = 0;
	uint16_t iS = 1, iB = 1;

	for (int i = 0; i <= m_considDim[0]; i++)  // 1st considered dimension
	{
		iB *= 2;
		iS = iB / 2;
	}
	m_partition += (m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]]);
	nB += (m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_xVal[m_considDim[0]]);
	nS += (m_xVal[m_considDim[0]] - m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]]);
	m_yVal = (m_targetDataSel[iS - 1] * nB + m_targetDataSel[iB - 1] * nS) / m_partition;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
void LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::interpolateInTwoDimensions()
{
	uint16_t iSS = 1, iBS = 1, iSB = 1, iBB = 1;
	T_STORAGE nSS, nBS, nSB, nBB;  // n(1stDimNotation)(2ndDimNotaion)

	for (int i = 0; i <= m_considDim[0]; i++)  // 1st considered dimension
	{
		iBS *= 2;
		iSS = iBS / 2;
	}
	for (int i = 0; i <= m_considDim[1]; i++)  // 2nd considered dimension
	{
		iBB *= 2;
	}
	iSB = iBB - iSS;

	m_partition = (m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]]) *
				  (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]]);

	nSS = (m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) * (m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]] - m_xVal[m_considDim[1]]);
	nBS = (m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) * (m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]] - m_xVal[m_considDim[1]]);
	nSB = (m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) * (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_xVal[m_considDim[1]]);
	nBB = (m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) * (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_xVal[m_considDim[1]]);
	m_yVal = (abs(nSS) * m_targetDataSel[iBB - 1] + abs(nBS) * m_targetDataSel[iSB - 1] + abs(nSB) * m_targetDataSel[iBS - 1] +
			  abs(nBB) * m_targetDataSel[iSS - 1]) /
			 m_partition;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
void LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::interpolateInThreeDimensions()
{
	// this part has to be generalized
	uint16_t iSSS = 1, iBSS = 2, iSBS = 3, iBBS = 4, iSSB = 5, iBSB = 6, iSBB = 7, iBBB = 8;
	T_STORAGE nSSS, nBSS, nSBS, nBBS, nSSB, nBSB, nSBB, nBBB;

	m_partition = (m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]]) *
				  (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]]) *
				  (m_dim[m_considDim[2]][m_iBVec[m_considDim[2]]] - m_dim[m_considDim[2]][m_iSVec[m_considDim[2]]]);

	nSSS = abs((m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iSVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));
	nBSS = abs((m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iSVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));
	nSBS = abs((m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iSVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));
	nBBS = abs((m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iSVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));
	nSSB = abs((m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iBVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));
	nBSB = abs((m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iSVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iBVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));
	nSBB = abs((m_dim[m_considDim[0]][m_iSVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iBVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));
	nBBB = abs((m_dim[m_considDim[0]][m_iBVec[m_considDim[0]]] - m_xVal[m_considDim[0]]) *
			   (m_dim[m_considDim[1]][m_iBVec[m_considDim[1]]] - m_xVal[m_considDim[1]]) *
			   (m_dim[m_considDim[2]][m_iBVec[m_considDim[2]]] - m_xVal[m_considDim[2]]));

	m_yVal = (nSSS * m_targetDataSel[iBBB - 1] + nBSS * m_targetDataSel[iSBB - 1] + nSBS * m_targetDataSel[iBSB - 1] + nBBS * m_targetDataSel[iSSB - 1] +
			  nSSB * m_targetDataSel[iBBS - 1] + nBSB * m_targetDataSel[iSBS - 1] + nSBB * m_targetDataSel[iBSS - 1] + nBBB * m_targetDataSel[iSSS - 1]) /
			 m_partition;
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
void LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::calculateNeighboringPoints()
{
	getPointIndecies();
	if (m_retValue)
	{
		switch (MAX_DIMENSIONS)
		{
			case 1:
				m_targetDataSel[0] = m_targetData[m_iSVec[0]];
				m_targetDataSel[1] = m_targetData[m_iBVec[0]];
				break;
			case 2:
				m_targetDataSel[0] = m_targetData[m_iSVec[1] + m_dimLen[1] * m_iSVec[0]];
				m_targetDataSel[1] = m_targetData[m_iSVec[1] + m_dimLen[1] * m_iBVec[0]];
				m_targetDataSel[2] = m_targetData[m_iBVec[1] + m_dimLen[1] * m_iSVec[0]];
				m_targetDataSel[3] = m_targetData[m_iBVec[1] + m_dimLen[1] * m_iBVec[0]];
				break;
			case 3:
				m_targetDataSel[0] = m_targetData[m_iSVec[2] + m_dimLen[2] * m_iSVec[1] + m_dimLen[1] * m_dimLen[2] * m_iSVec[0]];
				m_targetDataSel[1] = m_targetData[m_iSVec[2] + m_dimLen[2] * m_iSVec[1] + m_dimLen[1] * m_dimLen[2] * m_iBVec[0]];
				m_targetDataSel[2] = m_targetData[m_iSVec[2] + m_dimLen[2] * m_iBVec[1] + m_dimLen[1] * m_dimLen[2] * m_iSVec[0]];
				m_targetDataSel[3] = m_targetData[m_iSVec[2] + m_dimLen[2] * m_iBVec[1] + m_dimLen[1] * m_dimLen[2] * m_iBVec[0]];
				m_targetDataSel[4] = m_targetData[m_iBVec[2] + m_dimLen[2] * m_iSVec[1] + m_dimLen[1] * m_dimLen[2] * m_iSVec[0]];
				m_targetDataSel[5] = m_targetData[m_iBVec[2] + m_dimLen[2] * m_iSVec[1] + m_dimLen[1] * m_dimLen[2] * m_iBVec[0]];
				m_targetDataSel[6] = m_targetData[m_iBVec[2] + m_dimLen[2] * m_iBVec[1] + m_dimLen[1] * m_dimLen[2] * m_iSVec[0]];
				m_targetDataSel[7] = m_targetData[m_iBVec[2] + m_dimLen[2] * m_iBVec[1] + m_dimLen[1] * m_dimLen[2] * m_iBVec[0]];
				break;
			default:
				m_retValue = false;
				break;
		}
	}
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
void LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::getPointIndecies()
{
	for (uint8_t i = 0; i < MAX_DIMENSIONS; i++)
	{
		if (m_limitToBoundaryValues)
		{
			if (m_xVal[i] > m_dim[i][m_dimLen[i] - 1])
			{
				m_xVal[i] = m_dim[i][m_dimLen[i] - 1];
			}
			else if (m_xVal[i] < m_dim[i][0])
			{
				m_xVal[i] = m_dim[i][0];
			}
		}

		// finding targetData the index of the bigger index for each dimension.
		while ((m_dim[i][m_iBVec[i]] < m_xVal[i]) && (m_iBVec[i] < m_dimLen[i]))
		{
			m_iBVec[i]++;
		}
		// checks whether the chosen index is the same as the given
		// point for each dimension, and counts the active dimensions.
		if (m_dim[i][m_iBVec[i]] == m_xVal[i])
		{
			m_iSVec[i] = m_iBVec[i];
			m_dimReduction[i] = true;
		}
		else
		{
			m_iSVec[i] = m_iBVec[i] - 1;
			m_considDim[m_consideredDimCtr] = i;
			m_consideredDimCtr++;
		}
	}
	checkDimensionBoundaries();
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
void LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::checkDimensionBoundaries()
{
	for (uint8_t i = 0; i < MAX_DIMENSIONS; i++)
	{
		if ((m_iSVec[i] < 0) || (m_iBVec[i] >= m_dimLen[i]))
		{
			m_retValue = false;
		}
	}
}

template <uint8_t MAX_DIMENSIONS, typename T_IN, typename T_TARGET, typename T_STORAGE>
T_STORAGE LinearInterpolator<MAX_DIMENSIONS, T_IN, T_TARGET, T_STORAGE>::abs(T_STORAGE input)
{
	if (input < 0)
		input *= -1;
	return input;
}
} /* namespace semf */
#endif /* SEMF_UTILS_PROCESSING_LINEARINTERPOLATOR_H_ */
