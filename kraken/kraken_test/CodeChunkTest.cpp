#include "CodeChunkTest.h"

#include "AsmCodeHelper.h"

TEST_F(CodeChunkTest, ChunkIsEmptyBeginAndEndAreEqual)
{
  CodeChunk chunk;

  EXPECT_EQ( chunk.begin(), chunk.end() );
}

TEST_F(CodeChunkTest, BeginChunkIsNotEmptyReturnsIteratorToTheBeginningOfChunk)
{
  AsmCode disasm;
  disasm.VirtualAddr = _baseRva;

  EXPECT_TRUE( AsmCodeHelper::cmp( disasm, *_chunk.begin() ) );
}

TEST_F(CodeChunkTest, EndChunkIsNotEmptyReturnsIteratorToTheEndOfChunk)
{
  AsmCode disasm;
  disasm.VirtualAddr = _lastRva;

  EXPECT_TRUE( AsmCodeHelper::cmp( disasm,  *(_chunk.end() - 1) ) );
}

TEST_F(CodeChunkTest, FrontAlwaysReturnsFirstElement)
{
  AsmCode disasm;
  disasm.VirtualAddr = _baseRva;

  EXPECT_TRUE( AsmCodeHelper::cmp( disasm, _chunk.front() ) );
}

TEST_F(CodeChunkTest, BackAlwaysReturnsLastElement)
{
  AsmCode disasm;
  disasm.VirtualAddr = _lastRva;

  EXPECT_TRUE( AsmCodeHelper::cmp( disasm, _chunk.back() ) );
}

TEST_F(CodeChunkTest, FirstRvaChunkIsEmptyReturnsMinusOne)
{
  CodeChunk chunk;

  EXPECT_EQ( -1, chunk.first_rva() );
}

TEST_F(CodeChunkTest, LastRvaChunkIsEmptyReturnsMinusOne)
{
  CodeChunk chunk;

  EXPECT_EQ( -1, chunk.last_rva() );
}

TEST_F(CodeChunkTest, FirstRvaChunkIsNotEmptyReturnsRvaOfFirstDisasm)
{
  EXPECT_EQ( _baseRva, _chunk.first_rva() );
}

TEST_F(CodeChunkTest, LastRvaChunkIsNotEmptyReturnsRvaOfLastDisasm)
{
  EXPECT_EQ ( _lastRva, _chunk.last_rva() );
}

TEST_F(CodeChunkTest, IsAddressIncludedChunkIsEmptyReturnsFalse)
{
  CodeChunk chunk;

  EXPECT_FALSE( chunk.is_address_included( chunk.first_rva() ) );
}

TEST_F(CodeChunkTest, IsAddressIncludedAddressIsLessThanZeroReturnsFalse)
{
  CodeChunk chunk;

  EXPECT_EQ( false, chunk.is_address_included( -1 ) );
}

TEST_F(CodeChunkTest, IsAddressIncludedBoundaryRvasReturnsTrue)
{
  EXPECT_TRUE( _chunk.is_address_included( _baseRva ) );
  EXPECT_TRUE( _chunk.is_address_included( _lastRva ) );
}

TEST_F(CodeChunkTest, IsAddressIncludedAddressIsBetweenFirstRvaAndLastRvaReturnsTrue)
{
  EXPECT_TRUE( _chunk.is_address_included( _baseRva + 10 ) );
}

TEST_F(CodeChunkTest, IsAddressIncludedAddressIsNotBetweenFirstRvaAndLastRvaReturnsFalse)
{
  EXPECT_FALSE( _chunk.is_address_included( _baseRva - 1 ) );
  EXPECT_FALSE( _chunk.is_address_included( _lastRva + 1 ) );
}

TEST_F(CodeChunkTest, IntersectsWithPassedChunkBoundaryRvasOverlapWithInnerChunkReturnsTrue)
{
  CodeChunk passedChunk1 = GenerateCodeChunk( _baseRva - 10, _baseRva );
  CodeChunk passedChunk2 = GenerateCodeChunk( _lastRva, _lastRva + 10 );

  EXPECT_TRUE( _chunk.intersects_with( passedChunk1 ) );
  EXPECT_TRUE( _chunk.intersects_with( passedChunk2 ) );
}

TEST_F(CodeChunkTest, IntersectsWithPassedCodeChunkIntersectsReturnsTrue)
{
  CodeChunk passedChunk1 = GenerateCodeChunk( _baseRva - 10, _baseRva + 10 );
  CodeChunk passedChunk2 = GenerateCodeChunk( _baseRva + 10, _lastRva + 10 );

  EXPECT_TRUE( _chunk.intersects_with( passedChunk1 ) );
  EXPECT_TRUE( _chunk.intersects_with( passedChunk2 ) );
}

TEST_F(CodeChunkTest, IntersectsWithPassedCodeChunkDoesntIntersectReturnsFalse)
{
  CodeChunk passedChunk1 = GenerateCodeChunk( _baseRva - 10, _baseRva - 1 );
  CodeChunk passedChunk2 = GenerateCodeChunk( _lastRva + 1, _lastRva + 10 );

  EXPECT_FALSE( _chunk.intersects_with( passedChunk1 ) );
  EXPECT_FALSE( _chunk.intersects_with( passedChunk2 ) );
}

TEST_F(CodeChunkTest, IncludesCodeChunkIsEmptyReturnsFalse)
{
  CodeChunk chunk;

  EXPECT_FALSE( _chunk.includes( chunk ) );
}

TEST_F(CodeChunkTest, IncludesPassedChunkBoundaryRvasOverlapWithOnesFromInnerChunksReturnsTrue)
{
  CodeChunk passedChunk1 = GenerateCodeChunk( _baseRva, _lastRva - 10 );
  CodeChunk passedChunk2 = GenerateCodeChunk( _baseRva + 10, _lastRva );

  EXPECT_TRUE( _chunk.includes( passedChunk1 ) );
  EXPECT_TRUE( _chunk.includes( passedChunk2 ) );
}

TEST_F(CodeChunkTest, IncludesPassedChunkMatchesInnerChunkReturnsTrue)
{
  CodeChunk passedChunk = GenerateCodeChunk( _baseRva, _lastRva );

  EXPECT_TRUE( _chunk.includes( passedChunk ) );
}

TEST_F(CodeChunkTest, IncludesPassedChunkIsInnerChunksSubsetReturnsTrue)
{
  CodeChunk passedChunk = GenerateCodeChunk( _baseRva + 10, _lastRva - 10 );

  EXPECT_TRUE( _chunk.includes( passedChunk ) );
}

TEST_F(CodeChunkTest, IncludesPassedChunkPartiallyInetsectsWithInnerChunkReturnsFalse)
{
  CodeChunk passedChunk1 = GenerateCodeChunk( _baseRva - 10, _lastRva - 10 );
  CodeChunk passedChunk2 = GenerateCodeChunk( _baseRva + 10, _lastRva + 10 );

  EXPECT_FALSE( _chunk.includes( passedChunk1 ) );
  EXPECT_FALSE( _chunk.includes( passedChunk2 ) );
}

TEST_F(CodeChunkTest, IncludesPassedChunkNeverInetsectsWithInnerChunkReturnsFalse)
{
  CodeChunk passedChunk1 = GenerateCodeChunk( _baseRva - 10, _baseRva - 1 );
  CodeChunk passedChunk2 = GenerateCodeChunk( _lastRva + 1, _lastRva + 10 );

  EXPECT_FALSE( _chunk.includes( passedChunk1 ) );
  EXPECT_FALSE( _chunk.includes( passedChunk2 ) );
}

TEST_F(CodeChunkTest, AddToChunkAlwaysAddsSingleDisasm)
{
  AsmCode disasm;
  disasm.VirtualAddr = 1000;
  CodeChunk chunk;

  chunk.add_to_chunk( disasm );

  EXPECT_TRUE( AsmCodeHelper::cmp( disasm, chunk.front() ) );
}

TEST_F(CodeChunkTest, AddToChunkAlwaysAddsDisasmVector)
{
  CodeChunk::code_chunk_t disasmVector;

  AsmCode tempDisasm;
  for( int i = _baseRva; i <= _lastRva; ++i )
  {
    tempDisasm.VirtualAddr = i;
    disasmVector.push_back( tempDisasm );
  }
  
  CodeChunk chunk;

  chunk.add_to_chunk( disasmVector.begin(), disasmVector.end() );

  // There is no size() function for CodeChunk() yet so the only way we can get it's length is like this :)
  EXPECT_EQ( ( chunk.end() - chunk.begin() ), disasmVector.size() );

  auto vectorIterator = disasmVector.begin();
  auto chunkIterator = chunk.begin();
  while( vectorIterator != disasmVector.end() && chunkIterator == chunk.end() )
  {
    EXPECT_TRUE( AsmCodeHelper::cmp( *vectorIterator, *chunkIterator ) );

    ++vectorIterator;
    ++chunkIterator;
  }
}

CodeChunk CodeChunkTest::GenerateCodeChunk(const rva_t& firstRva, const rva_t& lastRva)
{
  CodeChunk chunk;
  
  AsmCode tempDisasm;
  for( auto passedChunkBaseRva = firstRva; passedChunkBaseRva <= lastRva; ++passedChunkBaseRva )
  {
    tempDisasm.VirtualAddr = passedChunkBaseRva;
    chunk.add_to_chunk( tempDisasm );
  }
  
  return chunk;
}