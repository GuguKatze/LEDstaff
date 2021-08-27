#include "Globals.h"
void vuFilter() {
  for(int i=0;i<7;i++){ 
    filteredLeftFast[i]  = 0.75 *  filteredLeftFast[i] + 0.25 *  vu.left[i];
    filteredRightFast[i] = 0.75 * filteredRightFast[i] + 0.25 * vu.right[i];
    filteredLeftSlow[i]  = 0.99 *  filteredLeftSlow[i] + 0.01 *  vu.left[i];
    filteredRightSlow[i] = 0.99 * filteredRightSlow[i] + 0.01 * vu.right[i];
  }
}
