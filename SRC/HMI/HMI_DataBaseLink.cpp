/******************************************************************************/
/*                                                                            */
/* Project N�  :  RB0505                                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Filename  : HMI_DataBaseLink.cpp                                    			*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/


/******************************************************************************/
/*%C                     Functionnal description :                            */
/*%C                                                                          */
/*%C Link DB																				      */
/*		                                                                        */
/******************************************************************************/

/******************************************************************************/
/*                           HEADER INCLUDE FILE		                  		*/
/******************************************************************************/

#include "HMI_DataBaseLink.hpp"

/******************************************************************************/
/*                           OTHER INCLUDE FILE		                  			*/
/******************************************************************************/

/******************************************************************************/
/*                           CONSTRUCTEUR/DESTRUCTEUR                			*/
/******************************************************************************/
DataBaseLink::DataBaseLink()
{
   SetIdParam(-1);
   SetDbTarget((e_TYPE_OF_DB_TARGET)0);
}
/*----------------------------------------------------------------------------*/
void DataBaseLink::Init(UWORD16 _IdParam, 
								e_TYPE_OF_DB_TARGET _DbTarget)
{
   SetIdParam(_IdParam);
   SetDbTarget(_DbTarget);
}

/******************************************************************************/
/*                           ACCESSEURS				                	   		*/
/******************************************************************************/
void DataBaseLink::SetIdParam(SWORD16 _IdParam)
{
   IdParam = _IdParam;
}
/*----------------------------------------------------------------------------*/
UWORD16 DataBaseLink::GetIdParam()
{
   return IdParam;
}
/*----------------------------------------------------------------------------*/
void DataBaseLink::SetDbTarget(e_TYPE_OF_DB_TARGET _DbTarget)
{
   DbTarget = _DbTarget;
}
/*----------------------------------------------------------------------------*/
e_TYPE_OF_DB_TARGET DataBaseLink::GetDbTarget()
{
   return DbTarget;
}