#pragma ident "$Id$"

/**
 * @file Variable.cpp
 * Class to define and handle 'descriptions' of GNSS variables.
 */

//============================================================================
//
//
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2007, 2008, 2009
//
//  2019-08-01 shjzhang
//
//============================================================================


#include "Variable.hpp"


using namespace utilSpace;

namespace gnssSpace
{

   // Default stochastic model to be assigned to variables.
   WhiteNoiseModel Variable::defaultModel;

      /* Common constructor for Variable.
       *  By default, it is indexed by SourceID.
       *
       * @param type             TypeID of variable.
       * @param pModel           Pointer to StochasticModel associated with
       *                         this variable. By default, it is a white
       *                         noise model.
       * @param sourceIndexed    Whether this variable is SourceID-indexed
       *                         or not. By default, it IS SourceID-indexed.
       * @param satIndexed       Whether this variable is SatID-indexed
       *                         or not. By default, it is NOT.
       * @param variance         Initial variance assigned to this variable.
       * @param coef             Default coefficient assigned.
       * @param forceCoef        Always use default coefficient.
       */
   Variable::Variable( const TypeID& type,
                       bool sourceIndexed,
                       bool satIndexed,
                       bool arcIndexed,
                       double variance,
                       double arc,
                       int  type_order,
					   int  now_index,
					   int  pre_index)
   {


      // Call Init method
      Init( type,
            variance,
            arc,
            type_order,
			now_index,
			pre_index);

      pVarModel = &defaultModel;

         // This couple lines override settings by Init.
      isSourceIndexed = sourceIndexed;
      isSatIndexed = satIndexed;
      isArcIndexed = arcIndexed;

   }  // End of 'Variable::Variable()'


      /* Common constructor for Variable.
       *  By default, it is indexed by SourceID.
       *
       * @param type             TypeID of variable.
       * @param pModel           Pointer to StochasticModel associated with
       *                         this variable. By default, it is a white
       *                         noise model.
       * @param sourceIndexed    Whether this variable is SourceID-indexed
       *                         or not. By default, it IS SourceID-indexed.
       * @param satIndexed       Whether this variable is SatID-indexed
       *                         or not. By default, it is NOT.
       * @param variance         Initial variance assigned to this variable.
       * @param coef             Default coefficient assigned.
       * @param forceCoef        Always use default coefficient.
       */
   Variable::Variable( const TypeID& type,
                       StochasticModel* pModel,
                       bool sourceIndexed,
                       bool satIndexed,
                       bool arcIndexed,
                       double variance,
                       double arc,
                       int type_order,
					   int  now_index,
					   int  pre_index)
   {


      // Call Init method
      Init( type,
            pModel,
            variance,
            arc,
            type_order,
			now_index,
			pre_index);

         // This couple lines override settings by Init.
      isSourceIndexed = sourceIndexed;
      isSatIndexed = satIndexed;
      isArcIndexed = arcIndexed;

   }  // End of 'Variable::Variable()'



      /* Initializing function
       *
       * @param type        TypeID of variable.
       * @param pModel      Pointer to StochasticModel associated with
       *                    this variable. By default, it is a white
       *                    noise model.
       * @param variance    Initial variance assigned to this variable.
       * @param coef        Default coefficient assigned.
       * @param forceCoef   Always use default coefficient.
       */
   void Variable::Init( const TypeID& type,
                        StochasticModel* pModel,
                        double variance,
                        double arc,
                        int type_order,
						int now_index,
                        int pre_index)
   {

      varType = type;

      if(pModel == NULL)
      {
         pVarModel = &defaultModel;
      }
      else
      {
         pVarModel = pModel;
      }

         // By default, it is source-indexed
      isSourceIndexed = true;

         // By default, it is not source indexed
      isSatIndexed = false;

         // By default, is is not arc-indexed
      isArcIndexed = false;

      satArc = arc;

         // Set initial variance
      initialVariance = variance;

	  m_now_index = now_index;

	  m_pre_index = pre_index;

      m_type_order = type_order;


	  return;

   }  // End of method 'Variable::Init()'


      /* Initializing function
       *
       * @param type        TypeID of variable.
       * @param pModel      Pointer to StochasticModel associated with
       *                    this variable. By default, it is a white
       *                    noise model.
       * @param variance    Initial variance assigned to this variable.
       * @param coef        Default coefficient assigned.
       * @param forceCoef   Always use default coefficient.
       */
   void Variable::Init( const TypeID& type,
                        double variance,
                        double arc,
                        int type_order,
						int now_index,
                        int pre_index)
   {

      varType = type;

         // By default, it is source-indexed
      isSourceIndexed = true;

         // By default, it is not source indexed
      isSatIndexed = false;

         // By default, is is not arc-indexed
      isArcIndexed = false;

      satArc = arc;

         // Set initial variance
      initialVariance = variance;

	  m_now_index = now_index;

	  m_pre_index = pre_index;

      m_type_order = type_order;

	  return;

   }  // End of method 'Variable::Init()'



      // Equality operator
   bool Variable::operator==(const Variable& right) const
   {

      return ( ( varType == right.getType() )          &&
               ( varSource == right.getSource() )      &&
               ( varSat == right.getSatellite() )      &&
               ( satArc == right.getArc() )  
               );            

   }  // End of 'Variable::operator=='



      // This ordering is somewhat arbitrary, but is required to be able
      // to use a Variable as an index to a std::map, or as part of a
      // std::set.
   bool Variable::operator<(const Variable& right) const
   {
       // Compare each field in turn
       if ( varSource == right.getSource() )
       {
           if( varSat == right.getSatellite())
           {
               if(varType==right.getType())
               {
                   return ( satArc < right.getArc() );
               }
               else
               {
                   return ( varType < right.getType() );
               }
           }
           else
           {
               return ( varSat < right.getSatellite() );
           }
       }
       else
       {
           return ( varSource < right.getSource() );
       }

   }  // End of 'Variable::operator<'



      // Assignment operator.
   Variable& Variable::operator=(const Variable& right)
   {

         // First check if these Variables are the same
      if ( this == &right ) return (*this);

         // If Variables are different, then set values of all fields
      setTypeOrder(right.getTypeOrder() );
      setType( right.getType() );
      setSource( right.getSource() );
      setSatellite( right.getSatellite() );
      setArc( right.getArc() );

      setModel( right.getModel() );

      setSourceIndexed( right.getSourceIndexed() );
      setSatIndexed( right.getSatIndexed() );
      setArcIndexed(right.getArcIndexed());              

      setInitialVariance( right.getInitialVariance() );


      setNowIndex(right.getNowIndex());

      setPreIndex(right.getPreIndex());

      return *this;

   }  // End of 'Variable::operator='


}  // End of namespace gnssSpace
