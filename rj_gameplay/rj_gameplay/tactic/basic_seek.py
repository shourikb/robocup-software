import stp
from rj_gameplay.role import seeker
from typing import List, Tuple
import numpy as np

from rj_msgs.msg import RobotIntent


class BasicSeek(stp.tactic.Tactic):
    """Seeks to a single point, passed in on init."""

    def __init__(self, world_state: stp.rc.WorldState, num_seekers: int):
        super().__init__(world_state)
        print(f"length_m: {world_state.field.length_m}")
        print(f"field len m: {world_state.field.length_m}")

        print(f"width_m: {world_state.field.width_m}")
        print(f"field width m: {world_state.field.width_m}")

        formation = self.get_x_formation(world_state)
        self._used_regions = []
        self._num_seekers = num_seekers

        for i in range(self._num_seekers):
            my_region = formation[i]
            self._used_regions.append(my_region)
            centroid = np.array(
                [
                    ((my_region[0] + my_region[2]) / 2),
                    ((my_region[1] + my_region[3]) / 2),
                ]
            )
            self._role_requests.append(
                (stp.role.cost.PickClosestToPoint(centroid), seeker.SeekerRole)
            )

    def get_x_formation(self, world_state: stp.rc.WorldState):
        y_quarter = world_state.field.length_m / 4
        y_3quarter = world_state.field.length_m - y_quarter
        field_y = world_state.field.length_m
        box_xright = world_state.field.def_area_x_right_coord
        box_xleft = world_state.field.def_area_x_left_coord
        field_xleft = world_state.field.bot_left_field_loc[0]
        field_xright = world_state.field.bot_right_field_loc[0]
        center_xleft = (
            world_state.field.center_field_loc[0] - world_state.field.center_diameter_m
        )
        center_xright = (
            world_state.field.center_field_loc[0] + world_state.field.center_diameter_m
        )
        center_yup = (
            world_state.field.center_field_loc[1] + world_state.field.center_diameter_m
        )
        center_ydown = (
            world_state.field.center_field_loc[0] - world_state.field.center_diameter_m
        )

        """
        Hard Code the Region Bounds
            -starting with the top left region being the first element, top right, center, bottom left, and then bottom right in order
            - bounds are ccw from bottom left (left_x, top_y, right_x, bottom_y)
        """
        X_formation = [
            # Region 1 bounds
            (field_xleft, field_y - 1, box_xleft, y_3quarter),
            # Region 2 bounds
            (box_xright, field_y - 1, field_xright, y_3quarter),
            # Region 3 bounds
            (center_xleft, center_yup, center_xright, center_ydown),
            # Region 4 bounds
            (field_xleft, y_quarter, box_xleft, 1),
            # Region 5 bounds
            (box_xright, y_quarter, field_xright, 1),
        ]

        return X_formation

    def init_roles(
        self,
        world_state: stp.rc.WorldState,
    ):
        for i, robot in enumerate(self.assigned_robots):
            role = self._role_requests[i][1]  # TODO: make this an actual type
            my_region = self._used_regions[i]
            if role is seeker.SeekerRole:
                self.assigned_roles.append(role(robot, my_region))

    def tick(
        self,
        world_state: stp.rc.WorldState,
    ) -> List[Tuple[int, RobotIntent]]:
        # returns list of (robot_id, robot_intent)

        # assumes all roles requested are filled, because tactic is one unit
        if len(self.assigned_roles) != len(self._role_requests):
            self.init_roles(world_state)

        return [(role.robot.id, role.tick(world_state)) for role in self.assigned_roles]

    def is_done(
        self,
        world_state: stp.rc.WorldState,
    ) -> bool:
        return all([role.is_done(world_state) for role in self.assigned_roles])
