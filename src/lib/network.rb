require 'ruby-fann'
require 'my_fann'

class Network
	def initialize (args)
		@network = MyFann.new(args)
	end
	
	def	train (data_train, max_epochs, neurons_between_reports, desired_error)
		@outputs = @network.train_on_data(data_train.data, max_epochs, neurons_between_reports, desired_error)
	end
	
	def run (inputs)
		@outputs = @network.run(inputs)
	end
	
	def	safe (filename)
		@network.safe(filename)
	end
end