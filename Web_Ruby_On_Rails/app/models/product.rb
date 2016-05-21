class Product < ActiveRecord::Base
  # attr_accessor :description, :image_url, :price, :title
  has_many :line_items
  has_many :orders, through: :line_items

  before_destroy :ensure_not_referenced_by_any_line_item

  PRODUCT_TYPES = ["Pizza", "Burge", "Dessert"]
  validates :title, :description, :image_url, :catagory, presence: true
  validates :catagory, inclusion: PRODUCT_TYPES
  validates :price, numericality: {greater_than_or_equal_to: 0.01}
  validates :title, uniqueness: true
  validates :image_url, allow_blank: true, format: {
  	multiline: true, 
    with:    %r{\.(gif|jpg|png)\Z}i,
    message: 'must be a URL for GIF, JPG or PNG image.'
  }

  private 
    # hook method, ensure there are no lines items refering to this product
  	def ensure_not_referenced_by_any_line_item
  		if line_items.empty?
  			return true
  		else
  			errors.add(:base, "Line Items present")
  			return false
  		end
  	end
end