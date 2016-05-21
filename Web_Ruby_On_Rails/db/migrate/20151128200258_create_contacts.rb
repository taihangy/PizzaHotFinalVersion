class CreateContacts < ActiveRecord::Migration
  def change
    create_table :contacts do |t|
      t.string :name
      t.string :email
      t.text :feedback

      t.timestamps null: false
    end
  end
end
