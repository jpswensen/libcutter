#include <stdint.h>
#include "device_cv_sim.hpp"
#include "types.h"

#include <opencv/highgui.h>

#define DPI_X 100
#define DPI_Y 100

#define DEFAULT_SIZE_X 8
#define DEFAULT_SIZE_Y 6

namespace Device
{

    CV_sim::CV_sim()
    {
        running = false;
        image = NULL;
        current_position.x = 0;
        current_position.y = 0;
    }

    CV_sim::CV_sim( const std::string filename )
    {
        output_filename = filename;
        running = false;
        image = NULL;
        current_position.x = 0;
        current_position.y = 0;
    }

    bool CV_sim::move_to(const xy & aPoint )
    {
        if( !running )
        {
            return false;
        }

        current_position = convert_to_internal( aPoint );
        return true;
    }

    bool CV_sim::cut_to(const xy & aPoint )
    {
        xy next_position;

        if( !running )
        {
            return false;
        }

        next_position = convert_to_internal( aPoint );

        if( image != NULL )
        {
            cvLine( image,
                cvPoint( current_position.x, current_position.y ),
                cvPoint(    next_position.x,    next_position.y ),
                                 /*grey           */
                cvScalar( 120, 120, 120 ),
                1,               /*thickness      */
                CV_AA,           /*antialiased    */
                0                /*fractional bits*/
                );
        }

        current_position = next_position;
        return true;
    }

    bool CV_sim::curve_to(const xy & p0, const xy & p1, const xy & p2, const xy & p3 )
    {
        if( !running )
        {
            return false;
        }
        return false;
    }

    bool CV_sim::start()
    {
        if( image == NULL )
        {
            image = cvCreateImage( cvSize( DPI_X * DEFAULT_SIZE_X, DPI_Y * DEFAULT_SIZE_Y ), IPL_DEPTH_8U, 1 );
        }
        running = true;
        return true;
    }

    bool CV_sim::stop()
    {
        if( image != NULL && output_filename.size() > 4 )
        {
            cvSaveImage( output_filename.c_str(), image );
            cvReleaseImage( &image );
        }
        running = false;
        return true;
    }

    xy CV_sim::convert_to_internal( const xy & input )
    {
        xy buf;

        buf.x = input.x * DPI_X;
        buf.y = input.y * DPI_Y;

        return buf;
    }

    xy CV_sim::get_dimensions( void )
    {
        xy buf;
        buf.x = DEFAULT_SIZE_X;
        buf.y = DEFAULT_SIZE_Y;
    }

}                                /* end namespace*/