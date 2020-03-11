from HaxballOnline.CirclePhysical import CirclePhysical


class Post(CirclePhysical):
    noChange = 0
    def __init__(self, game, px, py):
        super().__init__(game, px, py, 0, 1, 15, (0,0,0))
        self.noChange = 1
        self.to_sector_add()

    def __setattr__(self, key, value):
        if(self.noChange == 0):
            super(Post, self).__setattr__(key, value)