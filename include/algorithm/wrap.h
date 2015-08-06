/*
* Copyright 2015 G.C. Antonopoulos and B. Steltner, Laser Zentrum Hannover e.V.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* 
 * File:   wrap.h
 * Author: g.antonopoulos
 *
 * Created on 9. Mai 2014, 13:24
 */

#ifndef WRAP_H
#define	WRAP_H


/**
 * Wraps an angle in RADIANS to interval [-PI,PI]
 * @param angle
 * @return 
 */
float wrapToPi(float angle);

/**
 * Wraps an angle in RADIANS to interval [0,2 PI]
 * @param angle
 * @return 
 */
float wrapTo2Pi(float angle);

#endif	/* WRAP_H */

