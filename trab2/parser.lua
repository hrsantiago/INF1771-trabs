dofile('helpers.lua')
dofile('words.lua')

-- Arvores de decisao (trees/J48)
-- KNN (lazy/IBk)
-- SVM (functions/SMO)
-- Rede neural (functions/MultilayerPerceptron)


local p_words = {
  ['neg'] = {},
  ['pos'] = {},
}

local p_reviews = {
  ['neg'] = {},
  ['pos'] = {},
}

local p_usedWords = {}
local p_usedWordsCount = 0

function scandir(directory)
  local t = {}
  for filename in io.popen('ls "'..directory..'"'):lines() do
    table.insert(t, filename)
  end
  return t
end

function loadReviews(reviews, directory)
  local i = 0

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

    -- if i == 5000-1 then
    --   break
    -- end
    -- i = i + 1
  end
end

function processReviews()
  collectgarbage()

  for opinion, reviews in pairs(p_reviews) do
    print('Processing ', opinion)
    for index, review in ipairs(reviews) do
      if index % (math.floor(#reviews / 100)) == 0 then
        print('Processing ' .. 100 * index / #reviews .. '%')
      end
      processReview(review, opinion)
    end
    collectgarbage()
  end
end

function processReview(review, opinion)
  local text = review.text

  -- remove html stuff
  text, _ = string.gsub(text, '<.*>', ' ')

  -- lower text
  text = string.lower(text)

  -- remove ponctuation marks
  text, _ = string.gsub(text, ',', '')
  text, _ = string.gsub(text, '!', '')
  text, _ = string.gsub(text, '%.', '')
  text, _ = string.gsub(text, '%?', '')
  text, _ = string.gsub(text, '%-', '')
  text, _ = string.gsub(text, '%(', '')
  text, _ = string.gsub(text, '%)', '')
  text, _ = string.gsub(text, '%[', '')
  text, _ = string.gsub(text, '%]', '')
  text, _ = string.gsub(text, '%*', '')
  text, _ = string.gsub(text, '%%', '')
  text, _ = string.gsub(text, '%+', '')
  text, _ = string.gsub(text, '%$', '')
  text, _ = string.gsub(text, '/', '')
  text, _ = string.gsub(text, '"', '')

  -- remove numbers
  text, _ = string.gsub(text, '[0-9]', '')

  -- remove text after contractions
  text, _ = string.gsub(text, '\'[a-z]* ', ' ')

  -- add a space so first and last word can be removed
  text = ' ' .. text .. ' '

  -- remove black listed words
  for _,word in ipairs(blacklist) do
    text, _ = string.gsub(text, ' ' .. word .. ' ', ' ')
  end

  -- remove single letters
  text, _ = string.gsub(text, ' [a-z] ', ' ')

  -- remove double spaces
  repeat
    local count
    text, count = string.gsub(text, '  ', ' ')
  until count == 0

  -- remove first space
  text = string.sub(text, 2)

  -- remove last space
  text = string.sub(text, -#text)

  -- words
  local words = string.split(text)
  review.words = words

  -- pairs of words
  local wordsPairs = {}
  for i,word in ipairs(words) do
    local nextWord = words[i+1]
    if nextWord then
      table.insert(wordsPairs, word .. ' ' .. nextWord)
    end
  end
  review.wordsPairs = wordsPairs

  -- count
  for _,word in ipairs(words) do
    p_words[opinion][word] = (p_words[opinion][word] or 0) + 1
  end

  for _,word in ipairs(wordsPairs) do
    p_words[opinion][word] = (p_words[opinion][word] or 0) + 1
  end
end

function calculateWords()
  -- calc difference
  local p_wordsDiff = {}
  for opinion, words in pairs(p_words) do
    for word,count in pairs(words) do
      if opinion == 'pos' then
        p_wordsDiff[word] = (p_wordsDiff[word] or 0) + count
      else
        p_wordsDiff[word] = (p_wordsDiff[word] or 0) - count
      end
    end
  end

  for opinion, words in pairs(p_words) do
    local wordList = {}
    for word,count in pairs(p_wordsDiff) do
      local totalCount = p_words[opinion][word] or p_words['pos'][word] or p_words['neg'][word]
      if math.abs(count) / totalCount > 0.5 then -- at least 20% more on one side
        table.insert(wordList, { word = word, count = count })
      end
    end

    if opinion == 'pos' then
      table.sort(wordList, function(a, b) return a.count > b.count end)
    else
      table.sort(wordList, function(a, b) return a.count < b.count end)
    end

    print(opinion, ' words:')
    for i=1,100 do
      local word = wordList[i].word
      p_usedWordsCount = p_usedWordsCount + 1
      p_usedWords[word] = p_usedWordsCount
      print(i, word, wordList[i].count, p_words[opinion][word])
    end
  end

  for opinion, reviews in pairs(p_reviews) do
    for _, review in ipairs(reviews) do
      local bagOfWords = {}
      for _,word in ipairs(review.words) do
        local index = p_usedWords[word]
        if index then
          bagOfWords[index] = (bagOfWords[index] or 0) + 1
        end
      end

      for _,word in ipairs(review.wordsPairs) do
        local index = p_usedWords[word]
        if index then
          bagOfWords[index] = (bagOfWords[index] or 0) + 1
        end
      end

      review.bagOfWords = bagOfWords
    end
  end
end

function saveArff(filename)
  local file = io.open(filename, 'w')

  file:write('@relation reviews\n')
  file:write('\n')

  for i=1,p_usedWordsCount do
    file:write('@attribute count'..i..' real\n')
  end
  file:write('@attribute opinion {pos, neg}\n')
  file:write('\n')

  file:write('@data\n')
  for opinion, reviews in pairs(p_reviews) do
    for _, review in ipairs(reviews) do
      local words = ''
      for i=1,p_usedWordsCount do
        if i > 1 then
          words = words .. ','
        end
        words = words .. tostring(review.bagOfWords[i] or '0')
      end
      file:write(words .. ',' .. opinion .. '\n')
      file:flush()
    end
  end

  io.close(file)
end

function main()
  print('Loading...')
  loadReviews(p_reviews['neg'], 'dataset/original/part1/neg')
  loadReviews(p_reviews['neg'], 'dataset/original/part2/neg')
  loadReviews(p_reviews['pos'], 'dataset/original/part1/pos')
  loadReviews(p_reviews['pos'], 'dataset/original/part2/pos')

  print('Processing...')
  processReviews()

  print('Calculating...')
  calculateWords()

  saveArff('dataset/reviews.arff')
end

main()
