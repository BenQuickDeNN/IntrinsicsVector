/*************************************************
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#ifndef VEC_HPP

#define VEC_HPP

#include "../utilities/configure.hpp"

/**
 * Interface definition of Vec Class
 */
class Vec
{
public:
    inline virtual Vec & operator = (Vec& vec);
    
    /**
     * Load data from array to vector
     */
    virtual void load(const void* arr);
    /**
     * Store data from vector to array
     */
    virtual void store(void* arr);

    /**
     * Return the vector size (n * sizeof(type))
     */
    virtual int size();

    
};

#endif