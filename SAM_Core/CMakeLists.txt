# Copyright: (C) 2015 WYSIWYD Consortium
# Authors: Uriel Martinez
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.8)
set(superVisor samSupervisor.py)
set(optimiser samOptimiser.py)
set(train trainSAMModel.py)
set(interaction interactionSAMModel.py)

install(PROGRAMS ${superVisor} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)
yarp_install(PROGRAMS ${superVisor} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)

install(PROGRAMS ${optimiser} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)
yarp_install(PROGRAMS ${optimiser} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)

install(PROGRAMS ${train} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)
yarp_install(PROGRAMS ${train} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)

install(PROGRAMS ${interaction} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)
yarp_install(PROGRAMS ${interaction} DESTINATION bin PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE)
