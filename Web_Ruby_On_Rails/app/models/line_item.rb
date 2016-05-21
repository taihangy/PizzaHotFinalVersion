class LineItem < ActiveRecord::Base
	belongs_to :order
	belongs_to :product
	belongs_to :cart

	def total_price
		product.price * quantity
	end

	def decrement
	    if self.quantity > 1
	      self.quantity -= 1
	    else
	      self.destroy
	    end
	end

	def increment
		self.quantity += 1
	end
end
