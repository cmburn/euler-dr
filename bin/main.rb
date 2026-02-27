class Player
  attr_reader :position
  attr_reader :image

  def initialize(position:, image:)
    @position = position
    @image = image
  end

  def movement
    x = 0.0
    y = 0.0
    x += 1.0 if $app.keyboard.down? :d
    y += 1.0 if $app.keyboard.down? :w
    x -= 1.0 if $app.keyboard.down? :a
    y -= 1.0 if $app.keyboard.down? :d
    normalize [x, y]
  end

  def normalize(arr)
    denom = (arr[0] ** 2) + (arr[1] ** 2)
    arr.map { |n| n / denom }
  end

  def update(dt)
    dx, dy = movement.map { |n| n * dt }
    x += dx
    y += dy
  end
end

class State < Euler::Util::State
  SPRITE_PATH = 'assets/caveguy.png'
  def load
    @counter = 0
    @image = Euler::Graphics::Image.from_file(SPRITE_PATH)
    log.info "Loaded #{@image.width} x #{@image.height} image from " +
             SPRITE_PATH
  end


  def update(dt)
    @player ||= Player.new(
      position: [300, 300],
      image: @image
    )
    @player.update(dt)
  end

  # def input(ev)
  #   log.info "Received event #{ev.type}"
  #   if ev.type == :key_down
  #     case ev.key
  #     when :w then @sprite_y -= 1
  #     when :a then @sprite_x -= 1
  #     when :s then @sprite_y += 1
  #     when :d then @sprite_x += 1
  #     end
  #   end
  # end

  def draw
    @player.draw
  end

  def quit
    log.info "Received quit notification"
  end
end


# rubocop:disable Style/GlobalVars
$app = State.new
# rubocop:enable Style/GlobalVars

