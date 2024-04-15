/*
 * Recipe.h
 *
 *  Created on: 27/4/2016
 *      Author: Prototipado
 */

#ifndef INC_RECIPE_H_
#define INC_RECIPE_H_

#include "lpc_types.h"

/**
 *@brief
 */

uint16_t data_recipe;
uint16_t cont_recipe;

#define FIRST_RECIPE 9999
#define NEW_RECIPE   8888

#define NSAMPLES 60
#define ID_SPRAYER 4


typedef struct{
	char receta[4], producto[4], dosis[4], volumen[4], aplicador[4], regente[4];
}RCP_Recipe_T;

Status RCP_Init(void);
Status RCP_AddRecipe(RCP_Recipe_T* pRecipe);
Status RCP_RemoveRecipe(RCP_Recipe_T* pRecipe);
Status RCP_GetRecipe(RCP_Recipe_T* pRecipe);

#endif /* INC_RECIPE_H_ */
