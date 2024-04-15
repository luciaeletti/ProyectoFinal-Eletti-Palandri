/** @file Recipe.c
 *  @brief Persist sensors registered list.
 *
 *  is responsible for\n

 * @version		1.0
 * @date		14/01/2016
 * @author		Bioing. Martina Mauro
 * @note
 *  STGAA V3.0.0\n
 *  Facultad de Ingenieria - UNER.\n
 *  Laboratorio de prototipado Electronico &3D.\n
 *  All rights reserved. Copyright (C) 2016	.
 */

#include "Recipe.h"
#include "string.h"
#include "stdio.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
RCP_Recipe_T myRecipe;
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief
 * @param
 * @see
 * @return
 */
Status RCP_Init(void){

	strcpy(myRecipe.dosis    ,"");
	strcpy(myRecipe.aplicador,"");
	strcpy(myRecipe.producto ,"");
	strcpy(myRecipe.receta   ,"");
	strcpy(myRecipe.regente  ,"");
	strcpy(myRecipe.volumen  ,"");
	return SUCCESS;
}



/**
 * @brief
 * @param
 * @see
 * @return
 */
Status RCP_RemoveRecipe(RCP_Recipe_T* pRecipe){
	return SUCCESS;
}

/**
 * @brief
 * @param
 * @see
 * @return
 */
Status RCP_AddRecipe(RCP_Recipe_T* pRecipe){

	strcpy(myRecipe.receta,		pRecipe->receta);
	strcpy(myRecipe.producto,	pRecipe->producto);
	strcpy(myRecipe.dosis	,	pRecipe->dosis);
	strcpy(myRecipe.volumen ,	pRecipe->volumen);
	strcpy(myRecipe.aplicador,	pRecipe->aplicador);
	strcpy(myRecipe.regente ,	pRecipe->regente);

	return SUCCESS;
}

/**
 * @brief
 * @param
 * @see
 * @return
 */
Status RCP_GetRecipe(RCP_Recipe_T* pRecipe){

	strcpy(pRecipe->aplicador,myRecipe.aplicador);
	strcpy(pRecipe->regente	 ,myRecipe.regente);
	strcpy(pRecipe->receta	 ,myRecipe.receta);
	strcpy(pRecipe->producto ,myRecipe.producto);
	strcpy(pRecipe->dosis	 ,myRecipe.dosis);
	strcpy(pRecipe->volumen	 ,myRecipe.volumen);

	return SUCCESS;
}

