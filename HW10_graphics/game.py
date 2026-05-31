import random

WIDTH = 800
HEIGHT = 520

TITLE = "Poke Battle Arena"

battle_state = "title"  # title, selecting, action, enemy_turn, battle_over
selected_index = 0
message_queue = []
message_timer = 0.0

player_pokemon = None
enemy_pokemon = None

class Move:
    def __init__(self, name, power, accuracy, category, description):
        self.name = name
        self.power = power
        self.accuracy = accuracy
        self.category = category
        self.description = description

    def use(self, attacker, defender):
        hit_roll = random.random() <= self.accuracy
        if not hit_roll:
            return f"{attacker.name}'s {self.name} missed!"

        damage = max(1, int((attacker.attack * self.power / defender.defense) * random.uniform(0.85, 1.0)))
        defender.current_hp = max(0, defender.current_hp - damage)
        return f"{attacker.name} used {self.name}! {defender.name} lost {damage} HP."

class Creature:
    def __init__(self, name, hp, attack, defense, moves, color):
        self.name = name
        self.max_hp = hp
        self.current_hp = hp
        self.attack = attack
        self.defense = defense
        self.moves = moves
        self.color = color

    @property
    def hp_percent(self):
        return self.current_hp / self.max_hp if self.max_hp else 0

    def is_fainted(self):
        return self.current_hp <= 0

    def choose_move(self):
        return random.choice(self.moves)


def new_battle():
    global player_pokemon, enemy_pokemon, battle_state, selected_index, message_queue, message_timer

    player_pokemon = Creature(
        "Pyrofox",
        hp=120,
        attack=26,
        defense=20,
        moves=[
            Move("Flame Swipe", 18, 0.95, "physical", "A quick fire claw."),
            Move("Heat Wave", 20, 0.85, "special", "A strong flame blast."),
            Move("Quick Dash", 0, 1.00, "status", "Raises speed and avoids damage."),
            Move("Ember Beam", 12, 1.00, "special", "A small ember shot."),
        ],
        color=(220, 90, 35),
    )

    enemy_pokemon = Creature(
        "Aquadile",
        hp=110,
        attack=24,
        defense=22,
        moves=[
            Move("Splash Bite", 16, 0.90, "physical", "Chomps with wet teeth."),
            Move("Bubble Burst", 18, 0.90, "special", "A stream of bubbles."),
            Move("Wave Shield", 0, 1.00, "status", "Boosts defense temporarily."),
            Move("Tidal Rush", 20, 0.80, "special", "A powerful water surge."),
        ],
        color=(35, 125, 210),
    )

    battle_state = "selecting"
    selected_index = 0
    message_queue = ["A wild Aquadile appeared!", "Go, Pyrofox!"]
    message_timer = 0.0


def draw_health_bar(x, y, width, height, percent, color):
    background = Rect((x, y), (width, height))
    screen.draw.filled_rect(background, "gray20")
    fill_width = max(2, int(width * percent))
    fill_rect = Rect((x + 1, y + 1), (fill_width - 2, height - 2))
    screen.draw.filled_rect(fill_rect, color)
    screen.draw.rect(background, "white")


def draw():
    screen.fill((40, 40, 60))
    screen.draw.text(TITLE, center=(WIDTH / 2, 30), fontsize=50, color="white")

    if battle_state == "title":
        screen.draw.text("Press ENTER to start your battle.", midtop=(WIDTH / 2, 120), fontsize=36, color="lightgray")
        screen.draw.text("Use ARROW KEYS to choose moves and ENTER to confirm.", midtop=(WIDTH / 2, 180), fontsize=24, color="lightgray")
        return

    screen.draw.filled_rect(Rect((30, 70), (340, 180)), "black")
    screen.draw.filled_rect(Rect((430, 220), (340, 180)), "black")
    screen.draw.text(player_pokemon.name, topleft=(40, 80), fontsize=36, color="white")
    screen.draw.text(f"HP: {player_pokemon.current_hp}/{player_pokemon.max_hp}", topleft=(40, 120), fontsize=24, color="white")
    draw_health_bar(40, 150, 280, 24, player_pokemon.hp_percent, "green" if player_pokemon.hp_percent > 0.4 else "yellow" if player_pokemon.hp_percent > 0.15 else "red")
    screen.draw.text(enemy_pokemon.name, topleft=(440, 230), fontsize=36, color="white")
    screen.draw.text(f"HP: {enemy_pokemon.current_hp}/{enemy_pokemon.max_hp}", topleft=(440, 270), fontsize=24, color="white")
    draw_health_bar(440, 300, 280, 24, enemy_pokemon.hp_percent, "green" if enemy_pokemon.hp_percent > 0.4 else "yellow" if enemy_pokemon.hp_percent > 0.15 else "red")

    screen.draw.filled_rect(Rect((50, 330), (700, 150)), "black")
    screen.draw.rect(Rect((50, 330), (700, 150)), "white")

    text_y = 340
    for i, message in enumerate(message_queue[-3:]):
        screen.draw.text(message, topleft=(60, text_y + i * 32), fontsize=24, color="lightgray")

    if battle_state in ("selecting", "action"):
        draw_menu()
    elif battle_state == "enemy_turn":
        screen.draw.text("Enemy is choosing a move...", midbottom=(WIDTH / 2, HEIGHT - 20), fontsize=28, color="lightgray")
    elif battle_state == "battle_over":
        screen.draw.text("Battle Over! Press ENTER to restart.", midbottom=(WIDTH / 2, HEIGHT - 20), fontsize=32, color="white")


def draw_menu():
    menu_x = 460
    menu_y = 360
    menu_w = 260
    menu_h = 120
    screen.draw.filled_rect(Rect((menu_x, menu_y), (menu_w, menu_h)), "gray15")
    screen.draw.rect(Rect((menu_x, menu_y), (menu_w, menu_h)), "white")

    for idx, move in enumerate(player_pokemon.moves):
        y = menu_y + 10 + idx * 28
        text = f"{move.name}"
        color = "yellow" if idx == selected_index else "white"
        screen.draw.text(text, topleft=(menu_x + 10, y), fontsize=22, color=color)
        screen.draw.text(move.description, topleft=(menu_x + 130, y), fontsize=18, color="lightgray")


def add_message(text):
    global message_queue
    message_queue.append(text)
    if len(message_queue) > 6:
        message_queue = message_queue[-6:]


def on_key_down(key):
    global selected_index, battle_state

    if battle_state == "title":
        if key == keys.RETURN:
            new_battle()
        return

    if battle_state == "battle_over":
        if key == keys.RETURN:
            new_battle()
        return

    if battle_state == "selecting":
        if key == keys.UP:
            selected_index = (selected_index - 1) % len(player_pokemon.moves)
        elif key == keys.DOWN:
            selected_index = (selected_index + 1) % len(player_pokemon.moves)
        elif key == keys.RETURN:
            start_player_turn()


def start_player_turn():
    global battle_state, message_timer
    battle_state = "action"
    message_timer = 0.1
    move = player_pokemon.moves[selected_index]
    action_text = move.use(player_pokemon, enemy_pokemon)
    add_message(action_text)
    if enemy_pokemon.is_fainted():
        battle_state = "battle_over"
        add_message(f"{enemy_pokemon.name} fainted! You win!")
        return
    if move.category == "status":
        apply_status(move, player_pokemon)
    battle_state = "enemy_turn"
    message_timer = 0.8


def apply_status(move, target):
    if move.name == "Quick Dash":
        boost = max(1, int(target.attack * 0.15))
        target.attack += boost
        add_message(f"{target.name} gained {boost} Attack!")
    elif move.name == "Wave Shield":
        boost = max(1, int(target.defense * 0.18))
        target.defense += boost
        add_message(f"{target.name} gained {boost} Defense!")


def update(dt):
    global battle_state, message_timer
    if battle_state == "enemy_turn":
        message_timer -= dt
        if message_timer <= 0:
            perform_enemy_action()
    elif battle_state == "action":
        message_timer -= dt
        if message_timer <= 0:
            battle_state = "enemy_turn"
            message_timer = 0.3


def perform_enemy_action():
    global battle_state
    move = enemy_pokemon.choose_move()
    action_text = move.use(enemy_pokemon, player_pokemon)
    add_message(action_text)
    if player_pokemon.is_fainted():
        battle_state = "battle_over"
        add_message(f"{player_pokemon.name} fainted! You lost.")
        return
    if move.category == "status":
        apply_status(move, enemy_pokemon)
    battle_state = "selecting"


new_battle()
