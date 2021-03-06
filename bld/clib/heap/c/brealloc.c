/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "dll.h"        // needs to be first
#include "variety.h"
#include <malloc.h>
#include <string.h>
#include "heap.h"

_WCRTLINK void __based( void ) *_brealloc( __segment seg, void __based( void ) *mem, size_t size )
{
    void        __based( void ) *p;
    size_t      old_size;

    if( mem == _NULLOFF )
        return( _bmalloc( seg, size ) );
    if( size == 0 ) {
        _bfree( seg, mem );
        return( _NULLOFF );
    }
    old_size = _bmsize( seg, mem );
    p = _bexpand( seg, mem, size );
    if( p == _NULLOFF ) {               /* if it couldn't be expanded */
        p = _bmalloc( seg, size );      /* - allocate new block */
        if( p != _NULLOFF ) {           /* - if we got one */
            _fmemcpy( (void _WCFAR *)(seg :> p),
                      (void _WCFAR *)(seg :> mem),
                      old_size );
            _bfree( seg, mem );
        } else {
            _bexpand( seg, mem, old_size );
        }
    }
    return( p );
}
