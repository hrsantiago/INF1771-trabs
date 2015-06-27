-- Arvores de decisao (trees/J48)
-- KNN (lazy/IBk)
-- SVM (functions/SMO)
-- Rede neural (functions/MultilayerPerceptron)


local p_reviews = {
  ['neg'] = {},
  ['pos'] = {},
}

function scandir(directory)
  local t = {}
  for filename in io.popen('ls "'..directory..'"'):lines() do
    table.insert(t, filename)
  end
  return t
end

function loadReviews(reviews, directory)
  local files = scandir(directory)
  for _,filename in ipairs(files) do
    local file = io.open(directory .. '/' .. filename, 'r')
    local text = file:read('*all')
    io.close(file)

    local rating, _ = string.gsub(filename, '(.*_)([0-9]*)(.txt)', '%2')
    local data = {
      rating = tonumber(rating),
      text = text,
    }
    table.insert(reviews, data)
  end
end

function saveArff(filename)
  -- @relation weather

  -- @attribute outlook {sunny, overcast, rainy}
  -- @attribute temperature real
  -- @attribute humidity real
  -- @attribute windy {TRUE, FALSE}
  -- @attribute play {yes, no}

  -- @data
  -- sunny,85,85,FALSE,no
  -- sunny,80,90,TRUE,no

  local file = io.open(filename, 'w')

  file:write('@relation reviews\n')
  file:write('\n')

  file:write('@attribute rating real\n')
  file:write('@attribute status {pos, neg}\n')
  file:write('\n')

  file:write('@data\n')
  for status, reviews in pairs(p_reviews) do
    for _, review in ipairs(reviews) do
      file:write(review.rating .. ',' .. status .. '\n')
    end
  end

  io.close(file)
end

function main()
  loadReviews(p_reviews['neg'], 'dataset/original/part1/neg')
  loadReviews(p_reviews['neg'], 'dataset/original/part2/neg')
  loadReviews(p_reviews['pos'], 'dataset/original/part1/pos')
  loadReviews(p_reviews['pos'], 'dataset/original/part2/pos')
  saveArff('dataset/reviews.arff')
end

function dumpArray(array)
  for i=1,#t do print(i, t[i]) end
end

main()
