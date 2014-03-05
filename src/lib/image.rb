class Image
	attr_accessor :image_path, :gosu_image
	
	def initialize	(image_path)	
		@image_path = image_path
		self.gosu_image()
	end
	
	def	greyscale (r = 1.0, g = 1.0, b = 1.0)
		self.each do |pixel|
			sum = r*pixel[0] + g*pixel[1] + b*pixel[2]
			pixel[0] = sum/3
			pixel[1] = sum/3
			pixel[2] = sum/3
		end
	end

	def negative
		self.each do |pixel|
			pixel[0] = 1.0 - pixel[0]
			pixel[1] = 1.0 - pixel[1]
			pixel[2] = 1.0 - pixel[2]
		end
	end
	
	def positive
		self.negative()
	end
	
	def gosu_image (window = Gosu::Window.new(800, 600, false))
		@gosu_image = Gosu::Image.new(window, @image_path)
	end
	
	def each
		@gosu_image.each { |elem| yield elem } if block_given?
		true
	end
	
	def array
		@array = Array.new()
		self.each { |pixel| @array << pixel }
		@array
	end
	
	def width
		@gosu_image.width()
	end
	
	def height
		@gosu_image.height()
	end
	
	def resize
		# soon...
	end
end