/*****************************************************************************/
/*                                                                            */
/* Project N�  :  RB0505                                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Filename  : DIS_Number.c */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/


/******************************************************************************/
/*%C                     Functionnal description :                            */
/*%C                  */
/******************************************************************************/
/*%I Input Parameter :  NONE                                            */
/*%IO Input/Output :    NONE	                                              */
/*%O Output Parameter : NONE       */
/******************************************************************************/

/******************************************************************************/
/*                                INCLUDE FILES		                          */
/******************************************************************************/

#include "typedef.h"
#include "Driver_Display_Data.h"
#include "DIS_Number.h"
			 
/******************************************************************************/
/*                            FUNCTION BODY                                   */
/******************************************************************************/

void DIS_Number( SWORD32 value
					, UBYTE nature
					, SWORD16 line
					, UWORD16 column
					, UBYTE font
					, UBYTE nbdigit
					, UBYTE zero
					, UBYTE printon
					, UBYTE page )
{
UBYTE string[12];

SWORD32 temp_value;
SBYTE i,j;

// M�morisation de la valeur  
temp_value = value;
// Test si value n�gative travail sur une valeur positive uniquement
if (value < 0)
	{
	// Compl�ment � 2
	temp_value--;
	temp_value = ~temp_value;
	}

// D�placement de l'origine caract�re en bas � gauche
if ((font & ~INV) == ARIAL9)
{
	line -= 9;
}
else if ((font & ~INV) == ARIALBLACK11) 
{
	line -= 13;
}
else if ((font & ~INV) == ARIAL8) 
{
	line -= 9;
}
else if ((font & ~INV) == ARIALBOLD29) 
{
	line -= 34;
}
// Interdit les lignes n�gatives
if (line < 1) line = 1;

DRV_DIS_grSetPage ( page-1 ) ;

if ( font & INV )
	{	
	DRV_DIS_grNegative(1) ;
	DRV_DIS_grSetColor ( ON ) ;
	}
else
	{
	DRV_DIS_grNegative(0) ;
	DRV_DIS_grSetColor ( OFF ) ;
	}

if (zero == OFF)
	{
	// affichage entier positif ou n�gatif
   if (nature == INTEGER)
		{          
		// positionne la fin de caract�re
		string[10] = '\0';
		// Recherche des caract�res valides
		for(i = 9; i >= 0  ;i--)
			{
			string[i] = (UBYTE)(temp_value%10 + 48);
			temp_value /= 10;
			if (temp_value == 0) break;
			}

		// Test si valeur n�gative - Rajout du "-" devant le chiffre
		if (value < 0) string[--i] = '-';

		// R�ordonne le tablaeau de string
		for(j = i; j <= 10  ;j++)
			{
			string[j-i] = string[j];
			}
		}	
	// affichage d�cimal positif ou n�gatif
	else
		{
		// positionne la fin de caract�re
		string[10] = '\0';

		// recherche de la d�cimale
		string[9] = (UBYTE)(temp_value%10 + 48);
		// Suppression de la d�cimale
		temp_value /= 10;
		// Affichage du s�parateur
		string[8] = '.';
		
		// Recherche des caract�res valides
		for(i = 7; i >= 0  ;i--)
			{
			string[i] = (UBYTE)(temp_value%10 + 48);
			temp_value /= 10;
			if (temp_value == 0) break;
			}

		// Test si valeur n�gative - Rajout du "-" devant le chiffre
		if (value < 0) string[--i] = '-';

		// R�ordonne le tablaeau de string
		for(j = i; j <= 10  ;j++)
			{
			string[j-i] = string[j];
			}
		}
	}
// Zero = ON - Sans possibilit� de valeur n�gative
else
	{
		// positionne la fin de caract�re
		string[nbdigit] = '\0';
		// Recherche des caract�res valides
		for(i = nbdigit-1; i >= 0  ;i--)
			{
			string[i] = (UBYTE)(temp_value%10 + 48);
			temp_value /= 10;
			}
	}

DRV_DIS_grString ( (font & ~INV), column-1, (UBYTE)line-1, string, ~printon ) ;

}
