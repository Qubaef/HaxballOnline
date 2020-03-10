from HaxballOnline.CirclePhysical import CirclePhysical


class Post(CirclePhysical):
    def __init__(self, game, px, py):
        super().__init__(game, px, py, 0, 1, 15, (0,0,0))

    def __setattr__(self, key, value):
        return
