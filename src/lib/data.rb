require 'ruby-fann'

class Data
	attr_accessor :data
	def initialize (args)
		@data = RubyFann::TrainData.new(args)
	end
		
	def save (filename)
		@data.safe(filename)
	end
end