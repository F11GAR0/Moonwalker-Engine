#pragma once
#include "../walker.h"
/// Given a number of bits, return how many bytes are needed to represent that.
#define BITS_TO_BYTES(x) (((x)+7)>>3)
#define BYTES_TO_BITS(x) ((x)<<3)
#define BITSTREAM_STACK_ALLOCATION_SIZE 256

/// This class allows you to write and read native types as a string of bits.  BitStream is used extensively throughout RakNet and is designed to be used by users as well.	
/// \sa BitStreamSample.txt
class BitStream
{

public:
	template <class templateType>
	bool Serialize(bool writeToBitstream, templateType &var);

	template <class templateType>
	bool SerializeDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue);

	template <class templateType>
	bool SerializeDelta(bool writeToBitstream, templateType &currentValue);

	template <class templateType>
	bool SerializeCompressed(bool writeToBitstream, templateType &var);

	template <class templateType>
	bool SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue);

	/// Save as SerializeCompressedDelta(templateType &currentValue, templateType lastValue) when we have an unknown second parameter
	template <class templateType>
	bool SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue);

	bool Serialize(bool writeToBitstream,  char* input, const int numberOfBytes );

	template <class templateType> // templateType for this function must be a float or double
	bool SerializeNormVector(bool writeToBitstream,  templateType &x, templateType &y, templateType &z );

	template <class templateType> // templateType for this function must be a float or double
	bool SerializeVector(bool writeToBitstream,  templateType &x, templateType &y, templateType &z );

	template <class templateType> // templateType for this function must be a float or double
	bool SerializeNormQuat(bool writeToBitstream,  templateType &w, templateType &x, templateType &y, templateType &z);

	template <class templateType> // templateType for this function must be a float or double
	bool SerializeOrthMatrix(
		bool writeToBitstream,  
		templateType &m00, templateType &m01, templateType &m02,
		templateType &m10, templateType &m11, templateType &m12,
		templateType &m20, templateType &m21, templateType &m22 );

	bool SerializeBits(bool writeToBitstream, unsigned char* input, int numberOfBitsToSerialize, const bool rightAlignedBits = true );

	template <class templateType>
	void Write(templateType var);

	template <class templateType>
	void WriteDelta(templateType currentValue, templateType lastValue);

	template <class templateType>
	void WriteDelta(templateType currentValue);

	template <class templateType>
	void WriteCompressed(templateType var);

	template <class templateType>
	void WriteCompressedDelta(templateType currentValue, templateType lastValue);

	/// Save as WriteCompressedDelta(templateType currentValue, templateType lastValue) when we have an unknown second parameter
	template <class templateType>
	void WriteCompressedDelta(templateType currentValue);

	template <class templateType>
	bool Read(templateType &var);

	template <class templateType>
	bool ReadDelta(templateType &var);

	template <class templateType>
	bool ReadCompressed(templateType &var);

	template <class templateType>
	bool ReadCompressedDelta(templateType &var);

	template <class templateType> // templateType for this function must be a float or double
	void WriteNormVector( templateType x, templateType y, templateType z );

	/// Write a vector, using 10 bytes instead of 12.
	/// Loses accuracy to about 3/10ths and only saves 2 bytes, so only use if accuracy is not important.
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	template <class templateType> // templateType for this function must be a float or double
	void WriteVector( templateType x, templateType y, templateType z );

	/// Write a normalized quaternion in 6 bytes + 4 bits instead of 16 bytes.  Slightly lossy.
	/// \param[in] w w
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	template <class templateType> // templateType for this function must be a float or double
	void WriteNormQuat( templateType w, templateType x, templateType y, templateType z);

	/// Write an orthogonal matrix by creating a quaternion, and writing 3 components of the quaternion in 2 bytes each
	/// for 6 bytes instead of 36
	/// Lossy, although the result is renormalized
	template <class templateType> // templateType for this function must be a float or double
	void WriteOrthMatrix( 
		templateType m00, templateType m01, templateType m02,
		templateType m10, templateType m11, templateType m12,
		templateType m20, templateType m21, templateType m22 );

	inline int GetNumberOfBitsUsed( void ) const {return GetWriteOffset();}
	inline int GetWriteOffset( void ) const {return numberOfBitsUsed;}

	///Returns the length in bytes of the stream
	inline int GetNumberOfBytesUsed( void ) const {return BITS_TO_BYTES( numberOfBitsUsed );}

	///Returns the number of bits into the stream that we have read
	inline int GetReadOffset( void ) const {return readOffset;}

	// Sets the read bit index
	inline void SetReadOffset( int newReadOffset ) {readOffset=newReadOffset;}

	///Returns the number of bits left in the stream that haven't been read
	inline int GetNumberOfUnreadBits( void ) const {return numberOfBitsUsed - readOffset;}

	inline unsigned char* GetData( void ) const {return data;}

	BitStream()
	{
		numberOfBitsUsed = 0;
		//numberOfBitsAllocated = 32 * 8;
		numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE * 8;
		readOffset = 0;
		//data = ( unsigned char* ) malloc( 32 );
		data = (unsigned char*)stackData;
		copyData = true;
	}

	BitStream(int initialBytesToAllocate)
	{
		numberOfBitsUsed = 0;
		readOffset = 0;
		if (initialBytesToAllocate <= BITSTREAM_STACK_ALLOCATION_SIZE)
		{
			data = (unsigned char*)stackData;
			numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE * 8;
		}
		else
		{
			data = (unsigned char*)malloc(initialBytesToAllocate);
			numberOfBitsAllocated = initialBytesToAllocate << 3;
		}
		copyData = true;
	}

	BitStream(unsigned char* _data, unsigned int lengthInBytes, bool _copyData)
	{
		numberOfBitsUsed = lengthInBytes << 3;
		readOffset = 0;
		copyData = _copyData;
		numberOfBitsAllocated = lengthInBytes << 3;

		if (copyData)
		{
			if (lengthInBytes > 0)
			{
				if (lengthInBytes < BITSTREAM_STACK_ALLOCATION_SIZE)
				{
					data = (unsigned char*)stackData;
					numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE << 3;
				}
				else
				{
					data = (unsigned char*)malloc(lengthInBytes);
				}
#ifdef _DEBUG
				assert(data);
#endif
				memcpy(data, _data, lengthInBytes);
			}
			else
				data = 0;
		}
		else
			data = (unsigned char*)_data;
	}

	// Use this if you pass a pointer copy to the constructor (_copyData==false) and want to overallocate to prevent reallocation
	void SetNumberOfBitsAllocated(const unsigned int lengthInBits)
	{
		numberOfBitsAllocated = lengthInBits;
	}

	~BitStream()
	{
		if (copyData && numberOfBitsAllocated > BITSTREAM_STACK_ALLOCATION_SIZE << 3)
			free(data);  // Use realloc and free so we are more efficient than delete and new for resizing
	}

	void Reset(void)
	{
		// Note:  Do NOT reallocate memory because BitStream is used
		// in places to serialize/deserialize a buffer. Reallocation
		// is a dangerous operation (may result in leaks).

		if (numberOfBitsUsed > 0)
		{
			//  memset(data, 0, BITS_TO_BYTES(numberOfBitsUsed));
		}

		numberOfBitsUsed = 0;

		readOffset = 0;
	}

	// Write an array or casted stream
	void Write(const char* input, const int numberOfBytes)
	{
		if (numberOfBytes == 0)
			return;

		// Optimization:
		if ((numberOfBitsUsed & 7) == 0)
		{
			AddBitsAndReallocate(BYTES_TO_BITS(numberOfBytes));
			memcpy(data + BITS_TO_BYTES(numberOfBitsUsed), input, numberOfBytes);
			numberOfBitsUsed += BYTES_TO_BITS(numberOfBytes);
		}
		else
		{
			WriteBits((unsigned char*)input, numberOfBytes * 8, true);
		}

	}
	void Write(BitStream *bitStream)
	{
		Write(bitStream, bitStream->GetNumberOfBitsUsed());
	}
	void Write(BitStream *bitStream, int numberOfBits)
	{
		AddBitsAndReallocate(numberOfBits);
		int numberOfBitsMod8;

		while (numberOfBits-->0 && bitStream->readOffset + 1 <= bitStream->numberOfBitsUsed)
		{
			numberOfBitsMod8 = numberOfBitsUsed & 7;
			if (numberOfBitsMod8 == 0)
			{
				// New byte
				if (bitStream->data[bitStream->readOffset >> 3] & (0x80 >> (bitStream->readOffset++ % 8)))
				{
					// Write 1
					data[numberOfBitsUsed >> 3] = 0x80;
				}
				else
				{
					// Write 0
					data[numberOfBitsUsed >> 3] = 0;
				}
			}
			else
			{
				// Existing byte
				if (bitStream->data[bitStream->readOffset >> 3] & (0x80 >> (bitStream->readOffset++ % 8)))
					data[numberOfBitsUsed >> 3] |= 0x80 >> (numberOfBitsMod8); // Set the bit to 1
																			   // else 0, do nothing
			}

			numberOfBitsUsed++;
		}
	}

	// Read an array or casted stream
	bool Read(char* output, const int numberOfBytes)
	{
		// Optimization:
		if ((readOffset & 7) == 0)
		{
			if (readOffset + (numberOfBytes << 3) > numberOfBitsUsed)
				return false;

			// Write the data
			memcpy(output, data + (readOffset >> 3), numberOfBytes);

			readOffset += numberOfBytes << 3;
			return true;
		}
		else
		{
			return ReadBits((unsigned char*)output, numberOfBytes * 8);
		}
	}

	// Sets the read pointer back to the beginning of your data.
	void ResetReadPointer(void)
	{
		readOffset = 0;
	}

	// Sets the write pointer back to the beginning of your data.
	void ResetWritePointer(void)
	{
		numberOfBitsUsed = 0;
	}

	// Write a 0
	void Write0(void)
	{
		AddBitsAndReallocate(1);

		// New bytes need to be zeroed
		if ((numberOfBitsUsed & 7) == 0)
			data[numberOfBitsUsed >> 3] = 0;

		numberOfBitsUsed++;
	}

	// Write a 1
	void Write1(void)
	{
		AddBitsAndReallocate(1);

		int numberOfBitsMod8 = numberOfBitsUsed & 7;

		if (numberOfBitsMod8 == 0)
			data[numberOfBitsUsed >> 3] = 0x80;
		else
			data[numberOfBitsUsed >> 3] |= 0x80 >> (numberOfBitsMod8); // Set the bit to 1

		numberOfBitsUsed++;
	}

#ifdef _MSC_VER
#pragma warning( disable : 4800 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
	// Returns true if the next data read is a 1, false if it is a 0
	bool ReadBit(void)
	{
		return (bool)(data[readOffset >> 3] & (0x80 >> (readOffset++ & 7)));
	}

	// Align the bitstream to the byte boundary and then write the specified number of bits.
	// This is faster than WriteBits but wastes the bits to do the alignment and requires you to call
	// SetReadToByteAlignment at the corresponding read position
	void WriteAlignedBytes(const unsigned char* input,
		const int numberOfBytesToWrite)
	{
		AlignWriteToByteBoundary();
		Write((const char*)input, numberOfBytesToWrite);
	}

	// Read bits, starting at the next aligned bits. Note that the modulus 8 starting offset of the
	// sequence must be the same as was used with WriteBits. This will be a problem with packet coalescence
	// unless you byte align the coalesced packets.
	bool ReadAlignedBytes(unsigned char* output, const int numberOfBytesToRead)
	{
		if (numberOfBytesToRead <= 0)
			return false;

		// Byte align
		AlignReadToByteBoundary();

		if (readOffset + (numberOfBytesToRead << 3) > numberOfBitsUsed)
			return false;

		// Write the data
		memcpy(output, data + (readOffset >> 3), numberOfBytesToRead);

		readOffset += numberOfBytesToRead << 3;

		return true;
	}

	// Align the next write and/or read to a byte boundary.  This can be used to 'waste' bits to byte align for efficiency reasons
	void AlignWriteToByteBoundary(void)
	{
		if (numberOfBitsUsed)
			numberOfBitsUsed += 8 - (((numberOfBitsUsed - 1) & 7) + 1);
	}

	// Align the next write and/or read to a byte boundary.  This can be used to 'waste' bits to byte align for efficiency reasons
	void AlignReadToByteBoundary(void)
	{
		if (readOffset)
			readOffset += 8 - (((readOffset - 1) & 7) + 1);
	}

	// Write numberToWrite bits from the input source
	void WriteBits(const unsigned char *input, int numberOfBitsToWrite, const bool rightAlignedBits)
	{
		if (numberOfBitsToWrite <= 0)
			return;

		AddBitsAndReallocate(numberOfBitsToWrite);
		int offset = 0;
		unsigned char dataByte;
		int numberOfBitsUsedMod8;

		numberOfBitsUsedMod8 = numberOfBitsUsed & 7;

		// Faster to put the while at the top surprisingly enough
		while (numberOfBitsToWrite > 0)
			//do
		{
			dataByte = *(input + offset);

			if (numberOfBitsToWrite < 8 && rightAlignedBits)   // rightAlignedBits means in the case of a partial byte, the bits are aligned from the right (bit 0) rather than the left (as in the normal internal representation)
				dataByte <<= 8 - numberOfBitsToWrite;  // shift left to get the bits on the left, as in our internal representation

													   // Writing to a new byte each time
			if (numberOfBitsUsedMod8 == 0)
				* (data + (numberOfBitsUsed >> 3)) = dataByte;
			else
			{
				// Copy over the new data.
				*(data + (numberOfBitsUsed >> 3)) |= dataByte >> (numberOfBitsUsedMod8); // First half

				if (8 - (numberOfBitsUsedMod8) < 8 && 8 - (numberOfBitsUsedMod8) < numberOfBitsToWrite)   // If we didn't write it all out in the first half (8 - (numberOfBitsUsed%8) is the number we wrote in the first half)
				{
					*(data + (numberOfBitsUsed >> 3) + 1) = (unsigned char)(dataByte << (8 - (numberOfBitsUsedMod8))); // Second half (overlaps byte boundary)
				}
			}

			if (numberOfBitsToWrite >= 8)
				numberOfBitsUsed += 8;
			else
				numberOfBitsUsed += numberOfBitsToWrite;

			numberOfBitsToWrite -= 8;

			offset++;
		}
		// } while(numberOfBitsToWrite>0);
	}

	// Set the stream to some initial data.  For internal use
	void SetData(unsigned char *input)
	{
		data = input;
		copyData = false;
	}

	// Assume the input source points to a native type, compress and write it
	void WriteCompressed(const unsigned char* input,
		const int size, const bool unsignedData)
	{
		int currentByte = (size >> 3) - 1; // PCs

		unsigned char byteMatch;

		if (unsignedData)
		{
			byteMatch = 0;
		}

		else
		{
			byteMatch = 0xFF;
		}

		// Write upper bytes with a single 1
		// From high byte to low byte, if high byte is a byteMatch then write a 1 bit. Otherwise write a 0 bit and then write the remaining bytes
		while (currentByte > 0)
		{
			if (input[currentByte] == byteMatch)   // If high byte is byteMatch (0 of 0xff) then it would have the same value shifted
			{
				bool b = true;
				Write(b);
			}
			else
			{
				// Write the remainder of the data after writing 0
				bool b = false;
				Write(b);

				WriteBits(input, (currentByte + 1) << 3, true);
				//  currentByte--;


				return;
			}

			currentByte--;
		}

		// If the upper half of the last byte is a 0 (positive) or 16 (negative) then write a 1 and the remaining 4 bits.  Otherwise write a 0 and the 8 bites.
		if ((unsignedData && ((*(input + currentByte)) & 0xF0) == 0x00) ||
			(unsignedData == false && ((*(input + currentByte)) & 0xF0) == 0xF0))
		{
			bool b = true;
			Write(b);
			WriteBits(input + currentByte, 4, true);
		}

		else
		{
			bool b = false;
			Write(b);
			WriteBits(input + currentByte, 8, true);
		}
	}

	// Read numberOfBitsToRead bits to the output source
	// alignBitsToRight should be set to true to convert internal bitstream data to userdata
	// It should be false if you used WriteBits with rightAlignedBits false
	bool ReadBits(unsigned char* output, int numberOfBitsToRead, const bool alignBitsToRight = true)
	{
		if (numberOfBitsToRead <= 0)
			return false;

		if (readOffset + numberOfBitsToRead > numberOfBitsUsed)
			return false;

		int readOffsetMod8;

		int offset = 0;

		memset(output, 0, BITS_TO_BYTES(numberOfBitsToRead));

		readOffsetMod8 = readOffset & 7;

		// do
		// Faster to put the while at the top surprisingly enough
		while (numberOfBitsToRead > 0)
		{
			*(output + offset) |= *(data + (readOffset >> 3)) << (readOffsetMod8); // First half

			if (readOffsetMod8 > 0 && numberOfBitsToRead > 8 - (readOffsetMod8))   // If we have a second half, we didn't read enough bytes in the first half
				*(output + offset) |= *(data + (readOffset >> 3) + 1) >> (8 - (readOffsetMod8)); // Second half (overlaps byte boundary)

			numberOfBitsToRead -= 8;

			if (numberOfBitsToRead < 0)   // Reading a partial byte for the last byte, shift right so the data is aligned on the right
			{

				if (alignBitsToRight)
					* (output + offset) >>= -numberOfBitsToRead;

				readOffset += 8 + numberOfBitsToRead;
			}
			else
				readOffset += 8;

			offset++;

		}

		//} while(numberOfBitsToRead>0);

		return true;
	}

	// Assume the input source points to a compressed native type. Decompress and read it
	bool eadCompressed(unsigned char* output,
		const int size, const bool unsignedData)
	{
		int currentByte = (size >> 3) - 1;


		unsigned char byteMatch, halfByteMatch;

		if (unsignedData)
		{
			byteMatch = 0;
			halfByteMatch = 0;
		}

		else
		{
			byteMatch = 0xFF;
			halfByteMatch = 0xF0;
		}

		// Upper bytes are specified with a single 1 if they match byteMatch
		// From high byte to low byte, if high byte is a byteMatch then write a 1 bit. Otherwise write a 0 bit and then write the remaining bytes
		while (currentByte > 0)
		{
			// If we read a 1 then the data is byteMatch.

			bool b;

			if (Read(b) == false)
				return false;

			if (b)   // Check that bit
			{
				output[currentByte] = byteMatch;
				currentByte--;
			}
			else
			{
				// Read the rest of the bytes

				if (ReadBits(output, (currentByte + 1) << 3) == false)
					return false;

				return true;
			}
		}

		// All but the first bytes are byteMatch.  If the upper half of the last byte is a 0 (positive) or 16 (negative) then what we read will be a 1 and the remaining 4 bits.
		// Otherwise we read a 0 and the 8 bytes
		//assert(readOffset+1 <=numberOfBitsUsed); // If this assert is hit the stream wasn't long enough to read from
		if (readOffset + 1 > numberOfBitsUsed)
			return false;

		bool b;

		if (Read(b) == false)
			return false;

		if (b)   // Check that bit
		{

			if (ReadBits(output + currentByte, 4) == false)
				return false;

			output[currentByte] |= halfByteMatch; // We have to set the high 4 bits since these are set to 0 by ReadBits
		}
		else
		{
			if (ReadBits(output + currentByte, 8) == false)
				return false;
		}

		return true;
	}

	// Reallocates (if necessary) in preparation of writing numberOfBitsToWrite
	void BitStream::AddBitsAndReallocate(const int numberOfBitsToWrite)
	{
		if (numberOfBitsToWrite <= 0)
			return;

		int newNumberOfBitsAllocated = numberOfBitsToWrite + numberOfBitsUsed;

		if (numberOfBitsToWrite + numberOfBitsUsed > 0 && ((numberOfBitsAllocated - 1) >> 3) < ((newNumberOfBitsAllocated - 1) >> 3))   // If we need to allocate 1 or more new bytes
		{

			// Less memory efficient but saves on news and deletes
			newNumberOfBitsAllocated = (numberOfBitsToWrite + numberOfBitsUsed) * 2;
			//		int newByteOffset = BITS_TO_BYTES( numberOfBitsAllocated );
			// Use realloc and free so we are more efficient than delete and new for resizing
			int amountToAllocate = BITS_TO_BYTES(newNumberOfBitsAllocated);
			if (data == (unsigned char*)stackData)
			{
				if (amountToAllocate > BITSTREAM_STACK_ALLOCATION_SIZE)
				{
					data = (unsigned char*)malloc(amountToAllocate);

					// need to copy the stack data over to our new memory area too
					memcpy((void *)data, (void *)stackData, BITS_TO_BYTES(numberOfBitsAllocated));
				}
			}
			else
			{
				data = (unsigned char*)realloc(data, amountToAllocate);
			}
		}

		if (newNumberOfBitsAllocated > numberOfBitsAllocated)
			numberOfBitsAllocated = newNumberOfBitsAllocated;
	}

	// Should hit if reads didn't match writes
	void AssertStreamEmpty(void)
	{
		//assert(readOffset == numberOfBitsUsed);
	}

	// Exposes the data for you to look at, like PrintBits does.
	// Data will point to the stream.  Returns the length in bits of the stream.
	int BitStream::CopyData(unsigned char** _data) const
	{
		*_data = new unsigned char[BITS_TO_BYTES(numberOfBitsUsed)];
		memcpy(*_data, data, sizeof(unsigned char) * (BITS_TO_BYTES(numberOfBitsUsed)));
		return numberOfBitsUsed;
	}

	// Ignore data we don't intend to read
	void BitStream::IgnoreBits(const int numberOfBits)
	{
		readOffset += numberOfBits;
	}

	// Move the write pointer to a position on the array.  Dangerous if you don't know what you are doing!
	void BitStream::SetWriteOffset(const int offset)
	{
		numberOfBitsUsed = offset;
	}

	// If we used the constructor version with copy data off, this makes sure it is set to on and the data pointed to is copied.
	void BitStream::AssertCopyData(void)
	{
		if (copyData == false)
		{
			copyData = true;

			if (numberOfBitsAllocated > 0)
			{
				unsigned char * newdata = (unsigned char*)malloc(BITS_TO_BYTES(numberOfBitsAllocated));

				memcpy(newdata, data, BITS_TO_BYTES(numberOfBitsAllocated));
				data = newdata;
			}

			else
				data = 0;
		}
	}
	void ReverseBytes(unsigned char *input, unsigned char *output, int length)
	{
		for (int i = 0; i < length; i++)
			output[i] = input[length - i - 1];
	}


private:
	int numberOfBitsUsed;

	int numberOfBitsAllocated;

	int readOffset;

	unsigned char *data;

	/// true if the internal buffer is copy of the data passed to the constructor
	bool copyData;

	/// BitStreams that use less than BITSTREAM_STACK_ALLOCATION_SIZE use the stack, rather than the heap to store data.  It switches over if BITSTREAM_STACK_ALLOCATION_SIZE is exceeded
	unsigned char stackData[BITSTREAM_STACK_ALLOCATION_SIZE];
};

template <class templateType>
inline bool BitStream::Serialize(bool writeToBitstream, templateType &var)
{
	if (writeToBitstream)
		Write(var);
	else
		return Read(var);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue)
{
	if (writeToBitstream)
		WriteDelta(currentValue, lastValue);
	else
		return ReadDelta(currentValue);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeDelta(bool writeToBitstream, templateType &currentValue)
{
	if (writeToBitstream)
		WriteDelta(currentValue);
	else
		return ReadDelta(currentValue);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeCompressed(bool writeToBitstream, templateType &var)
{
	if (writeToBitstream)
		WriteCompressed(var);
	else
		return ReadCompressed(var);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue)
{
	if (writeToBitstream)
		WriteCompressedDelta(currentValue,lastValue);
	else
		return ReadCompressedDelta(currentValue);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue)
{
	if (writeToBitstream)
		WriteCompressedDelta(currentValue);
	else
		return ReadCompressedDelta(currentValue);
	return true;
}

inline bool BitStream::Serialize(bool writeToBitstream, char* input, const int numberOfBytes )
{
	if (writeToBitstream)
		Write(input, numberOfBytes);
	else
		return Read(input, numberOfBytes);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeNormVector(bool writeToBitstream, templateType &x, templateType &y, templateType &z )
{
	if (writeToBitstream)
		WriteNormVector(x,y,z);
	else
		return ReadNormVector(x,y,z);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeVector(bool writeToBitstream,  templateType &x, templateType &y, templateType &z )
{
	if (writeToBitstream)
		WriteVector(x,y,z);
	else
		return ReadVector(x,y,z);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeNormQuat(bool writeToBitstream,  templateType &w, templateType &x, templateType &y, templateType &z)
{
	if (writeToBitstream)
		WriteNormQuat(w,x,y,z);
	else
		return ReadNormQuat(w,x,y,z);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeOrthMatrix(
	bool writeToBitstream,  
	templateType &m00, templateType &m01, templateType &m02,
	templateType &m10, templateType &m11, templateType &m12,
	templateType &m20, templateType &m21, templateType &m22 )
{
	if (writeToBitstream)
		WriteOrthMatrix(m00,m01,m02,m10,m11,m12,m20,m21,m22);
	else
		return ReadOrthMatrix(m00,m01,m02,m10,m11,m12,m20,m21,m22);
	return true;
}

inline bool BitStream::SerializeBits(bool writeToBitstream, unsigned char* input, int numberOfBitsToSerialize, const bool rightAlignedBits )
{
	if (writeToBitstream)
		WriteBits(input,numberOfBitsToSerialize,rightAlignedBits);
	else
		return ReadBits(input,numberOfBitsToSerialize,rightAlignedBits);
	return true;
}

template <class templateType>
inline void BitStream::Write(templateType var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var)==1)
		WriteBits( ( unsigned char* ) & var, sizeof( templateType ) * 8, true );
	else
	{
		WriteBits( ( unsigned char* ) & var, sizeof(templateType) * 8, true );
	}
}


/// Write any integral type to a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] var The value to write
template <class templateType>
inline void BitStream::WriteCompressed(templateType var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var)==1)
		WriteCompressed( ( unsigned char* ) & var, sizeof( templateType ) * 8, true );
	else
	{
		WriteCompressed( ( unsigned char* ) & var, sizeof(templateType) * 8, true );
	}
}

template <>
inline void BitStream::WriteCompressed(bool var)
{
	Write(var);
}

/// For values between -1 and 1
template <>
inline void BitStream::WriteCompressed(float var)
{
	if (var < -1.0f)
		var=-1.0f;
	if (var > 1.0f)
		var=1.0f;
	Write((unsigned short)((var+1.0f)*32767.5f));
}

/// For values between -1 and 1
template <>
inline void BitStream::WriteCompressed(double var)
{
	if (var < -1.0f)
		var=-1.0f;
	if (var > 1.0f)
		var=1.0f;
#ifdef _DEBUG
	assert(sizeof(unsigned long)==4);
#endif
	Write((unsigned long)((var+1.0)*2147483648.0));
}

/// Write any integral type to a bitstream.  If the current value is different from the last value
/// the current value will be written.  Otherwise, a single bit will be written
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] currentValue The current value to write
/// \param[in] lastValue The last value to compare against
template <class templateType>
inline void BitStream::WriteCompressedDelta(templateType currentValue, templateType lastValue)
{
	if (currentValue==lastValue)
	{
		Write(false);
	}
	else
	{
		Write(true);
		WriteCompressed(currentValue);
	}
}

/// Write a bool delta.  Same thing as just calling Write
/// \param[in] currentValue The current value to write
/// \param[in] lastValue The last value to compare against
template <>
inline void BitStream::WriteCompressedDelta(bool currentValue, bool lastValue)
{
#ifdef _MSC_VER
#pragma warning(disable:4100)   // warning C4100: 'lastValue' : unreferenced formal parameter
#endif
	Write(currentValue);
}

/// Save as WriteCompressedDelta(templateType currentValue, templateType lastValue) when we have an unknown second parameter
template <class templateType>
inline void BitStream::WriteCompressedDelta(templateType currentValue)
{
	Write(true);
	WriteCompressed(currentValue);
}

/// Save as WriteCompressedDelta(bool currentValue, templateType lastValue) when we have an unknown second bool
template <>
inline void BitStream::WriteCompressedDelta(bool currentValue)
{
	Write(currentValue);
}

/// Read any integral type from a bitstream.  Define __BITSTREAM_NATIVE_END if you need endian swapping.
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::Read(templateType &var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var)==1)
		return ReadBits( ( unsigned char* ) &var, sizeof(templateType) * 8, true );
	else
	{
		return ReadBits( ( unsigned char* ) & var, sizeof(templateType) * 8, true );
	}
}

/// Read any integral type from a bitstream.  If the written value differed from the value compared against in the write function,
/// var will be updated.  Otherwise it will retain the current value.
/// ReadDelta is only valid from a previous call to WriteDelta
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::ReadDelta(templateType &var)
{
	bool dataWritten;
	bool success;
	success=Read(dataWritten);
	if (dataWritten)
		success=Read(var);
	return success;
}

/// Read a bool from a bitstream
/// \param[in] var The value to read
template <>
inline bool BitStream::ReadDelta(bool &var)
{
	return Read(var);
}

/// Read any integral type from a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::ReadCompressed(templateType &var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var)==1)
		return ReadCompressed( ( unsigned char* ) &var, sizeof(templateType) * 8, true );
	else
	{
		return ReadCompressed( ( unsigned char* ) & var, sizeof(templateType) * 8, true );
	}
}

template <>
inline bool BitStream::ReadCompressed(bool &var)
{
	return Read(var);
}

/// For values between -1 and 1
template <>
inline bool BitStream::ReadCompressed(float &var)
{
	unsigned short compressedFloat;
	if (Read(compressedFloat))
	{
		var = ((float)compressedFloat / 32767.5f - 1.0f);
		return true;
	}
	return false;
}

/// For values between -1 and 1
template <>
inline bool BitStream::ReadCompressed(double &var)
{
	unsigned long compressedFloat;
	if (Read(compressedFloat))
	{
		var = ((double)compressedFloat / 2147483648.0 - 1.0);
		return true;
	}
	return false;
}


/// Read any integral type from a bitstream.  If the written value differed from the value compared against in the write function,
/// var will be updated.  Otherwise it will retain the current value.
/// the current value will be updated.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// ReadCompressedDelta is only valid from a previous call to WriteDelta
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::ReadCompressedDelta(templateType &var)
{
	bool dataWritten;
	bool success;
	success=Read(dataWritten);
	if (dataWritten)
		success=ReadCompressed(var);
	return success;
}

/// Read a bool from a bitstream
/// \param[in] var The value to read
template <>
inline bool BitStream::ReadCompressedDelta(bool &var)
{
	return Read(var);
}