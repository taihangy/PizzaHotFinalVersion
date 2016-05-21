class Order < ActiveRecord::Base
	has_many :line_items, dependent: :destroy
	
	PAYMENT_TYPES = ['Credit card', 'Cash', 'Check']
	validates :name, :address, :phone, :email, presence: true
	validates :pay_type, inclusion: PAYMENT_TYPES
	
	def add_line_items_from_cart(cart)
		cart.line_items.each do |item|
			item.cart_id = nil
			line_items << item
		end
	end

	def total_price
		line_items.to_a.sum do |line|
			line.total_price
		end
	end

	def ship
		self.status = "shipped"
	end
end
