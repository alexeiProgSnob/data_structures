#ifndef __BIG_NUMBER_H__
#define __BIG_NUMBER_H__

#include <stdint.h> /*< uint8_t >*/
#include <stddef.h> /*< size_t >*/

typedef struct BigNumber BigNumber;

BigNumber* BigNumberCreate(uint8_t* _bigNumber, size_t _size);

uint8_t* BigNumberDestroy(BigNumber** _bigNumber, size_t* _bufferSize);

BigNumber* BigNumberMultiplication(BigNumber* _firstNumber, BigNumber* _secondNumber);

#endif /* __BIG_NUMBER_H__ */