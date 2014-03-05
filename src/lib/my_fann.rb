requre 'ruby-fann'

class MyFann < RubyFann::Standard
  def training_callback (args)
    puts "ARGS: #{args.inspect}"
    0  
  end
end