from CirclePhysical import CirclePhysical

class Player(CirclePhysical):

    def __init__(self, game, px, py):
        super().__init__(game, px, py, 0, 1, 15, (0,0,0))
        self.to_sector_add()

    def update(self):
        return 1
