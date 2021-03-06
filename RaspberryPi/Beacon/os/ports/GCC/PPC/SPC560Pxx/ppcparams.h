/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    SPC560Pxx/ppcparams.h
 * @brief   PowerPC parameters for the SPC560Pxx.
 *
 * @defgroup PPC_SPC560Pxx SPC560Pxx/MPC560x Specific Parameters
 * @ingroup PPC_SPECIFIC
 * @details This file contains the PowerPC specific parameters for the
 *          SPC560Pxx/MPC560x platform.
 * @{
 */

#ifndef _PPCPARAMS_H_
#define _PPCPARAMS_H_

/**
 * @brief   PPC core model.
 */
#define PPC_VARIANT             PPC_VARIANT_e200z0

/**
 * @brief   IVORx registers support.
 */
#define PPC_SUPPORTS_IVORS      FALSE

/**
 * @brief   Book E instruction set support.
 */
#define PPC_SUPPORTS_BOOKE      FALSE

/**
 * @brief   VLE instruction set support.
 */
#define PPC_SUPPORTS_VLE        TRUE

/**
 * @brief   Supports VLS Load/Store Multiple Volatile instructions.
 */
#define PPC_SUPPORTS_VLE_MULTI  TRUE

#endif /* _PPCPARAMS_H_ */

/** @} */
