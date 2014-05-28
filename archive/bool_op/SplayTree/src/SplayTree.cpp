
	
        /**
         * Construct the tree.
         */
    

        /**
         * Copy constructor.
         */
   

        /**
         * Destructor.
         */
      

        /**
         * Insert x into the tree.
         */
      

        /**
         * Remove x from the tree.
         */
       

        /**
         * Find the smallest item in the tree.
         * Not the most efficient implementation (uses two passes), but has correct
         *     amortized behavior.
         * A good alternative is to first call Find with parameter
         *     smaller than any item in the tree, then call findMin.
         * Return the smallest item or ITEM_NOT_FOUND if empty.
         */
       

        /**
         * Find the largest item in the tree.
         * Not the most efficient implementation (uses two passes), but has correct
         *     amortized behavior.
         * A good alternative is to first call Find with parameter
         *     larger than any item in the tree, then call findMax.
         * Return the largest item or ITEM_NOT_FOUND if empty.
         */



        /**
         * Find item x in the tree.
         * Return the matching item or ITEM_NOT_FOUND if not found.
         */
       


        /**
         * Make the tree logically empty.
         */
     
      

        /**
         * Test if the tree is logically empty.
         * @return true if empty, false otherwise.
         */

      

        /**
         * Print the tree contents in sorted order.
         */
       

        /**
         * Deep copy.
         */
    

        /**
         * Internal method to perform a top-down splay.
         * The last accessed node becomes the new root.
         * This method may be overridden to use a different
         * splaying algorithm, however, the splay tree code
         * depends on the accessed item going to the root.
         * x is the target item to splay around.
         * t is the root of the subtree to splay.
         */
    
      

        /**
         * Rotate binary tree node with left child.
         */
     

        /**
         * Rotate binary tree node with right child.
         */


        /**
         * Internal method to print a subtree t in sorted order.
         * WARNING: This is prone to running out of stack space.
         */
      

        /**
         * Internal method to reclaim internal nodes in subtree t.
         * WARNING: This is prone to running out of stack space.
         */
     

        /**
         * Internal method to clone subtree.
         * WARNING: This is prone to running out of stack space.
         */

    
