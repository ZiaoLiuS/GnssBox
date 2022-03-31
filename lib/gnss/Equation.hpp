
/**
 * @file Equation.hpp
 * GNSS Data Structure to define and handle 'descriptions' of GNSS equations.
 *
 * 2020/02/12
 * add addVariable(VariableSet& varSet);
 *
 */

#ifndef EQUATION_HPP
#define EQUATION_HPP

#include "Exception.hpp"
#include "DataStructures.hpp"
#include "StochasticModel.hpp"
#include "Variable.hpp"


using namespace utilSpace;

namespace gnssSpace
{

      /// Defines a header containing basic equation data
   struct equationHeader
   {
      /// Source this equation is related to
      SourceID equationSource;

      /// Satellite this equation is related to
      SatID equationSat;

      /// Independent term
      Variable indTerm;

      int orderIndex;

         /// satellite system 
      SatelliteSystem satSys;


         /** Constant weight associated to this equation. This is a relative
          *  value that compares with the other Equations. It is 1.0 by
          *  default.
          */
      double constWeight;

      typeValueMap typeValueData;

         /// Default constructor
      equationHeader()
         : equationSource("unknown"), equationSat("00"),
           constWeight(1.0), satSys(SatelliteSystem::GPS), orderIndex(-1)
       {};


         /** Explicit constructor
          *
          * @param source     Source this equation is related to.
          * @param sat        Satellite this equation is related to.
          * @param indep      Variable representing the independent term.
          * @param cweight    Constant weight associated to this equation.
          */
      equationHeader( const SourceID& source,
                      const SatID& sat,
                      const Variable indep,
                      const double& cweight ,
                      const int& index,
                      const SatelliteSystem::Systems system)
         : equationSource(source), equationSat(sat), indTerm(indep),
           constWeight(cweight), orderIndex(index), satSys(system)
       {};


         /** Explicit constructor from a Variable
          *
          * @param indep      Variable representing the independent term.
          */
      equationHeader(const Variable& indep,
                     SatelliteSystem::Systems sys=SatelliteSystem::GPS)
         : equationSource("unknown"), equationSat("00"),
           indTerm(indep), satSys(SatelliteSystem::GPS), constWeight(1.0) {};


         /// Assignment operator
      virtual equationHeader& operator=(const equationHeader& right);


         /** Assignment operator from a Variable
          *
          * @param indep      Variable representing the independent term.
          */
      virtual equationHeader& operator=(const Variable& indep)
      { indTerm = indep; return (*this); };


         /// Destructor
      virtual ~equationHeader() {};


   }; // End of struct 'equationHeader'



      /** GNSS Data Structure to define and handle 'descriptions' of GNSS
       *  equations.
       */
   struct Equation : gnssData<equationHeader, VarCoeffMap>
   {

         /// Default constructor.
      Equation();


         /** Common constructor. It defines an Equation from its header. You
          *  must later use other methods to input the variables.
          *
          * @param head     Data structure describing the Equation header.
          */
      Equation( const equationHeader& head )
      { header = head; };


         /** Common constructor. It defines an Equation from its independent
          *  term. You must later use other methods to input the variables.
          *
          * @param indep     Variable object describing the independent term.
          */
      Equation( const Variable& indep, 
                SatelliteSystem::Systems sys=SatelliteSystem::GPS
                );


         /** Common constructor. It defines an Equation from its independent
          *  term. You must later use other methods to input the variables.
          *
          * @param var     TypeID object describing the independent term.
          */
      Equation( const TypeID& type, 
                SatelliteSystem::Systems sys=SatelliteSystem::GPS,
                const int& index=-1);


         /// Return the independent term of this equation
      virtual TypeID getIndepType() const
      { return header.indTerm.getType(); };

         /// Return the independent term of this equation
      virtual Variable getIndependentTerm() const
      { return header.indTerm; };

         /** Set the independent term of this Equation.
          *
          * @param var     Variable object describing the independent term.
          */
      virtual Equation& setIndependentTerm(const Variable& var)
      { header = var; return (*this); };


         /// Get the value of the constant weight associated to this equation
      virtual double getWeight() const
      { return header.constWeight; };


         /** Set the value of the constant weight associated to this equation
          *
          * @param cweight    Value of constant weight.
          */
      virtual Equation& setWeight(const double& cweight)
      { header.constWeight = cweight; return (*this); };


      virtual Equation& setOrderIndex(const double& index)
      { header.orderIndex= index; return (*this); };

      virtual int getOrderIndex()
      { return header.orderIndex; };

         /** Add a variable (unknown) to this Equation
          *
          * @param var     Variable object to be added to the unknowns.
          */
      virtual Equation& addVariable(const Variable& var,
                                    const Coefficient& coef)
      { 
          body.insert(make_pair(var,coef)); 
          return (*this); 
      };

         /** Add a variable (unknown) to this Equation
          *
          * @param var     Variable object to be added to the unknowns.
          */
      virtual Equation& addVariable(const Variable& var,
                                    const double coef )
      {


             // Coefficient object
          Coefficient coefData;
          coefData.setConstCoeff(coef);

             // Insert new variable and coefData into the body
          body.insert(make_pair(var, coefData));

          return (*this);
      };

      virtual Equation& addVariable(const Variable& var)
      {
             // Coefficient object
          Coefficient coefData;
          coefData.setType(var.getType());


             // Insert new variable and coefData into the body
          body.insert(make_pair(var, coefData));

          return (*this);
      };

      virtual Equation& addVarSet(const VariableSet& varSet)
      { 
          for(auto var: varSet)
          {
              addVariable(var);
          }

          return (*this); 
      };

      virtual VariableSet getVarSet()
      {
          VariableSet tempVarSet;
          for(auto vc: (*this).body )
          {
              tempVarSet.insert(vc.first);
          }

          return tempVarSet;
      };


         /** Remove a variable (unknown) from this Equation
          *
          * @param var     Variable object to be romoved from the unknowns.
          */
      virtual Equation& removeVariable(const Variable& var)
      { body.erase(var); return (*this); };


         /** Remove ALL variables (unknowns) from this Equation.
          *
          * @warning This method does NOT clear the Equation's independent
          *          term. You MUST take care of it yourself (use method
          *          'setIndependentTerm', for instance).
          */
      virtual Equation& clear()
      { body.clear(); return (*this); };


         /// Get equation SourceID.
      SourceID getEquationSource() const
      { return header.equationSource; };

      	/// Get equation SatID.
      SatID getEquationSat() const
      { return header.equationSat; };

      	/// Get equation SatID.
      SatelliteSystem getSystem() const
      { return header.satSys; };


      virtual std::ostream& dump( std::ostream& s ) const
      {
          s << " source:" << header.equationSource 
            << " satellite:" << header.equationSat
            << " indep var:" << header.indTerm
            << " system:" << header.satSys
            << endl;

          for(auto it=(*this).body.begin(); it != (*this).body.end(); it++)
          {
              s << "unknown:" <<  (*it).first 
                << "coeff:" << (*it).second 
                << endl;
          }

          return s;
      };

         /// This ordering is somewhat arbitrary, but is required to be able
         /// to use an Equation as an index to a std::map, or as part of a
         /// std::set.
      virtual bool operator<(const Equation& right) const
      {   
          if (header.orderIndex == right.header.orderIndex)
          {
              if (header.equationSource == right.header.equationSource )
              {
                  if(header.equationSat == right.header.equationSat)
                  {
                      return (header.indTerm < right.header.indTerm);
                  }
                  else
                  {
                     return (header.equationSat < right.header.equationSat);
                  }
              }
              else
              {
                  return (header.equationSource < right.header.equationSource);
              }
             
          }
          else
          {
              return (header.orderIndex < right.header.orderIndex); 
          }
      };

      /// Destructor
      virtual ~Equation() {};

   }; // End of struct 'Equation'

   /// stream output for Variable
   inline std::ostream& operator<<(std::ostream& s, const Equation& v)
   {
       v.dump(s);
       return s;
   }

}  // End of namespace gnssSpace

#endif   // EQUATION_HPP
