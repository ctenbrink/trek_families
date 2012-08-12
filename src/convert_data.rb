#!/usr/bin/env ruby
require 'roo'
require 'fileutils'
require 'csv'
require 'yaml'

def convert_xlsx_to_csv
    working_dir = File.expand_path("../data/youth");
    Dir.chdir(working_dir) do
        Dir.glob("*.csv") do |file|
            FileUtils.rm file
        end
        Dir.glob("#{working_dir}/*.xlsx") do |file|
            puts "Converting \'#{file}\' to csv..."
            xlsx = Excelx.new(file)
            xlsx.to_csv("#{File.dirname(file)}/#{File.basename(file, ".xlsx")}.csv")
        end
    end
end

def get_unit(filename)
    unit = "--unknown--"
    unit = 'LL1' if filename =~ /.*Lakeland\s*1.*/
    unit = 'LL2' if filename =~ /.*Lakeland\s*2.*/
    unit = 'H1' if filename =~ /.*Hayden\s*1.*/
    unit = 'H2' if filename =~ /.*Hayden\s*2.*/
    unit = 'H3' if filename =~ /.*Hayden\s*3.*/
    unit = 'H4' if filename =~ /.*Hayden\s*4.*/
    unit = 'K' if filename =~ /.*Kellog.*/
    unit = 'W' if filename =~ /.*Wallace.*/
    unit = 'DG' if filename =~ /.*Dalton.*/
    unit
end

def get_height_and_weight(data)
    ht_wt = [0.0, 0.0]
    unless  data.nil? or data.empty?
        ht_wt_elements = data.split("\"")
        if ht_wt_elements.size < 2 and data[-1] != "\"" 
            ht_wt_elements = data.split("\'")
        end
        height =(ht_wt_elements.size >= 1 ? ht_wt_elements[0].strip : '0')
        weight =(ht_wt_elements.size >= 2 ? ht_wt_elements[1].strip : '0')
    end

    unless height.nil?
        height.strip!
        height.delete! ' '
        match = height.match(/(\d*)(\'*)(\d*)/)
        inches = 0.0
        if(2 <= match.length and match.length <=3)
            inches = match[1].to_f
        elsif(match.length > 3 )
            inches = (match[1].to_f * 12.0) + match[3].to_f
        end
        ht_wt[0] = inches
    end

    unless weight.nil?    
        weight.strip!
        weight.delete! '#'
        ht_wt[1] = weight.to_f
    end

    ht_wt
end

def get_mas_and_pas
    groups = []
    groups << {'name' => 'Doe', 'unit' => 'H1', 'has_medical_training' => true}
    groups_ary = []
    groups.each do |grp|
        hsh = {'Group'=>grp}
        groups_ary << hsh
    end
    groups_ary
end

def convert_csv_to_yaml
    working_dir = File.expand_path("../data/youth");
    unassigned_people = []
    Dir.chdir(working_dir) do
       Dir.glob("#{working_dir}/*.csv") do |file|
            puts "Converting \'#{file}\' to yaml..."
            unit = get_unit(file)
            data = CSV.read(file)
            is_male = false
            col_map = {}
            data.each do |row|
                next if row.empty? or row[0].nil?
                break if row[0].downcase.strip == 'not registered'
                if row[0].downcase.strip == 'yw'
                    is_male = false 
                    row.each_index do |idx|
                        col_map[:name] = 0
                        next if row[idx].nil?
                        col_map[:age] = idx if(row[idx].downcase.include?('age'))
                        col_map[:htwt] = idx if(row[idx].downcase.include?('ht'))
                        col_map[:med] = idx if(row[idx].downcase.include?('med'))
                        col_map[:allergy] = idx if(row[idx].downcase.include?('allerg'))
                        col_map[:yc] = idx if(row[idx].downcase.include?('yc'))
                    end
                    next
                end
                if row[0].downcase.strip == 'ym'
                    is_male = true 
                    next
                end
                next if col_map.empty?

                raise "Expected to find 6 named columns, but only found #{col_map.size}" unless col_map.size == 6
                
                name = row[col_map[:name]]
                next if name.empty?
                name_elements = name.split(/,/)
                if name_elements.size < 2
                    name_elements = name.split(" ")
                end
                if name_elements.size < 2
                    name_elements = name.split('.')
                end
                first = (name_elements.size >= 2 ? name_elements[1].strip : '')
                last =  (name_elements.size >= 1 ? name_elements[0].strip : '')

                age = row[col_map[:age]].to_i

                ht_wt = get_height_and_weight(row[col_map[:htwt]])

                raise "Expected two values for height and weight" unless ht_wt.size == 2

                height = ht_wt[0]
                weight = ht_wt[1]
                
                med = row[col_map[:med]]
                allergy = row[col_map[:allergy]]
               
                medical_condition = []
                medical_condition << med unless med.nil? or med.empty?
                medical_condition << "allergy - #{allergy}" unless allergy.nil? or allergy.empty?

                on_youth_committee = (row[col_map[:yc]].nil? or row[col_map[:yc]].empty?) ? false : true

                person = {}
                person['first_name'] = first
                person['last_name'] = last
                person['is_male'] = is_male
                person['age'] = age
                person['height'] = height
                person['weight'] = weight
                person['ward'] = unit
                person['medical_condition'] = medical_condition.join(', ')
                person['on_youth_committee'] = on_youth_committee
                hsh = {}
                hsh['Person'] = person
                unassigned_people << hsh

                puts person.to_yaml
            end
        end
    end
    groups = get_mas_and_pas 
    document = {'unassigned_people'=> unassigned_people, 'groups'=>groups}
    document_hsh = {'Document'=>document}
    puts document_hsh.to_yaml
    File.open(File.join(working_dir, 'trek.yaml'), 'w') do |file|
        file.puts document_hsh.to_yaml
    end
end

convert_xlsx_to_csv
convert_csv_to_yaml

