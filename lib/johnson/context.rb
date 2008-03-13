module Johnson
  class Context
    attr_reader :delegate
    
    def initialize(delegate=Johnson::SpiderMonkey::Context)
      @delegate = delegate.is_a?(Class) ? delegate.new : delegate
    end
    
    def [](key)
      delegate[key.to_s]
    end
    
    def []=(key, value)
      delegate[key.to_s] = value
    end
    
    def evaluate(expression)
      return nil if expression.nil?
      delegate.evaluate(expression)
    end
  end
end