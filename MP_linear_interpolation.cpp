#include <iostream>
#include <iomanip>
#include <vector>
#include <omp.h>
using namespace std;


int main()
{
   omp_set_num_threads (16);
   // Original data

   int i;
   vector<double> xData(100001);
   // for(i = 1; i <= 100000; i++){
   //    xData.push_back((int)i * 5);
      
   // }
   #pragma omp parallel for
    for (int i = 1; i <= 100000; i++) {
        xData[i]= i*5;
    }
   
   vector<double> yData(100001);
   double y = 0.3;
   // for(i = 1; i <= 100000; i++){
   //    y += 0.2;
   //    yData.push_back(y);
   // }
   #pragma omp parallel for shared(y)
    for (int i = 1; i <= 100000; i++) {
        y += 0.2;
        yData[i] = y;
    }

   // Set up some points for interpolation in xVals
   const int NPTS = xData[xData.size() - 1];
   vector<double> xVals;
   for ( i = 1; i <= NPTS; i++ ) xVals.push_back( (int)i );
   vector<double> yVals;
   for (  i = 1; i <= NPTS; i++ ) yVals.push_back( 0 );

   
   #pragma omp parallel shared(yVals, NPTS) private(i)
   // #pragma omp parallel shared(xData, yData, xVals, yVals, NPTS) private(i)
   {

      #pragma omp for 
      // Interpolate
      for ( i = 0; i < NPTS; i++ )
      {
         int size = xData.size();
         int counter = 0;                                                                  // find left end of interval for interpolation
         if ( xVals[i] >= xData[size - 2] )                                                 // special case: beyond right end
         {
            counter = size - 2;
         }
         else
         {
            while ( xVals[i] > xData[counter+1] ) counter++;
         }
         double xL = xData[counter], yL = yData[counter], xR = xData[counter+1], yR = yData[counter+1];
         if ( xVals[i] < xL ) {
            yR = yL;
         }
         if ( xVals[i] > xR ) {
            yL = yR;
         }
         double dydx = ( yR - yL ) / ( xR - xL );
         double y = yL + dydx * ( xVals[i] - xL );
         yVals[i] = y;
      }

   }
   

   // Output
   // cout << "Original data:\n";
   // for ( int i = 0; i < xData.size(); i++ ) cout << fixed << setw( 15 ) << setprecision( 6 ) << xData[i] << fixed << setw( 15 ) << setprecision( 6 ) << yData[i] << '\n';
   // cout << "\nInterpolated data:\n";
   // for ( int i = 0; i < xVals.size(); i++ ) cout << fixed << setw( 15 ) << setprecision( 6 ) << xVals[i] << fixed << setw( 15 ) << setprecision( 6 ) << yVals[i] << '\n';
}
