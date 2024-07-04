##############################################################################
#                                                                            #
# eini - library for parsing ini files.                                      #
#                                                                            #
#  Copyright (C) 2024 Kuzmin Yaroslav <kuzmin.yaroslav@gmail.com>            #
#                                                                            #
# eini is free software: you can redistribute it and/or modify it            #
# under the terms of the GNU General Public License as published by the      #
# Free Software Foundation, either version 3 of the License, or              #
# (at your option) any later version.                                        #
#                                                                            #
# eini is distributed in the hope that it will be useful, but                #
# WITHOUT ANY WARRANTY; without even the implied warranty of                 #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       #
# See the GNU General Public License for more details.                       #
#                                                                            #
# You should have received a copy of the GNU General Public License along    #
# with this program.  If not, see <http://www.gnu.org/licenses/>.            #
#                                                                            #
##############################################################################

project('eini', 'c', version:'0.1', meson_version:'>=1.4.0'
       ,default_options:['warning_level=1', 'buildtype=debug', 'b_sanitize=address,undefined', 'c_std=c99'])

source = ['sample.c', 'eini.c', 'eini.h']

binary = executable(meson.project_name(), source)

test('basic', binary) 

