class StoreController < ApplicationController
	skip_before_filter :authorize
  def index
  	@products = Product.order(:title)
  	@count = increment_count
  	@cart = current_cart
  	@show_message = "You've been here #{@count} times" if @count > 5
  end
end
